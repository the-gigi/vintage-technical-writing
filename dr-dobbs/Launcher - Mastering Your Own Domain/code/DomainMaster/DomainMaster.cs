using System;
using System.Collections;
using System.Collections.Specialized;
using System.Diagnostics;
using System.Reflection;
using System.IO;
using System.Threading;
using System.Runtime.InteropServices;

namespace DomainMasterLib
{
	public delegate	int ExecuteAssemblyDelegate(string assembly);
	
	internal class AppInfo : IAppInfo
	{
		public AppInfo(string path)
		{
			m_path = path;
		}

		public string Path { get { return m_path; } }
		public AppDomain Domain 
		{ 
			get { return m_domain; }
			set { m_domain = value; }
		}

		public State State 
		{ 
			get { return m_state; }
			set { m_state = value; }
		}
		
		string		m_path;
		AppDomain	m_domain = null;
		State		m_state = State.IDLE;	
	};

	public class DomainMaster : IDomainMaster
	{
		static public IDomainMaster Instance
		{
			get 
			{
				if (m_instance == null)
				{					
					m_instance = new DomainMaster(Directory.GetCurrentDirectory());
				}
				return m_instance; 
			} 
		}

		private DomainMaster()
		{
		}

		private DomainMaster(string dir)
		{
			m_directory = dir;
		}

		public void AttachSink(IDomainMasterEvents sink)
		{
			m_sink = sink;
		}

		public bool AddApp(string app, string path)
		{
			if (app.Length == 0 || path.Length == 0)
				return false;

			if (m_apps.Contains(app))
				return false;
			
			m_apps[app] = new AppInfo(path);
			return true;
		}

		public void Launch(string app)
		{
			if (!m_apps.Contains(app))
				return;

			AppInfo ai = m_apps[app] as AppInfo;
			AppDomainSetup ads = new AppDomainSetup();
			ads.ApplicationBase = Path.GetDirectoryName(ai.Path);			
			Directory.SetCurrentDirectory(ads.ApplicationBase);
			ads.LoaderOptimization = LoaderOptimization.MultiDomainHost;
			ai.Domain = AppDomain.CreateDomain(app, null, ads);			
			AppDomain ad = ai.Domain;
			ad.SetData("Sink", m_sink);
			ad.UnhandledException += new UnhandledExceptionEventHandler(UnhandledExceptionHandler);
			ad.AssemblyLoad += new AssemblyLoadEventHandler(AssemblyLoadHandler);
			ad.DomainUnload += new EventHandler(DomainUnloadHandler);

			Console.WriteLine("Launch({0}), PrivateBinPath={1}, AppBase={2}", ad.FriendlyName, ad.SetupInformation.PrivateBinPath, ad.SetupInformation.ApplicationBase);
			ExecuteAssemblyDelegate ead = new ExecuteAssemblyDelegate(ad.ExecuteAssembly);
			ead.BeginInvoke(ai.Path, null, null);

			ai.State = State.RUNNING;
		}

		public void Terminate(string app)
		{
			if (!m_apps.Contains(app))
				return;

			AppInfo ai = m_apps[app] as AppInfo;
			try
			{
				ai.State = State.SHUTTING_DOWN;
				AppDomain.Unload(ai.Domain);
				ai.Domain = null;
				ai.State = State.IDLE;
			}
			catch (CannotUnloadAppDomainException e)
			{
				Console.WriteLine("{0}\n{1}", e.Message, e.StackTrace);
			}
		}
   
		static public void UnhandledExceptionHandler(object sender, UnhandledExceptionEventArgs e)
		{
			Exception ex = e.ExceptionObject as Exception;
			if (ex is AppDomainUnloadedException) // we expect this exception of course
				return;

			Console.WriteLine("{0}\n{1}\n", ex.Message, ex.StackTrace);
		}

		static public void AssemblyLoadHandler(object sender, AssemblyLoadEventArgs args) 
		{
			Debug.Assert(sender as _AppDomain == AppDomain.CurrentDomain);
			try 
			{
				AppDomain ad = sender as AppDomain;
				IDomainMasterEvents sink = ad.GetData("Sink") as IDomainMasterEvents;
				if (sink != null)
				{	
					string assembly = args.LoadedAssembly.GetName().Name;			
					sink.OnAssemblyLoad(ad.FriendlyName, assembly);
				}
			}
			catch (Exception e) 
			{
				Console.WriteLine("{1}\n{2}", e.Message, e.StackTrace);
			}							
		}

		static public void DomainUnloadHandler(object sender, EventArgs args) 
		{
			try 
			{
				AppDomain ad = sender as AppDomain;
				IDomainMasterEvents sink = ad.GetData("Sink") as IDomainMasterEvents;
				if (sink != null)
					sink.OnDomainUnload(ad.FriendlyName);
			}
			catch (Exception e) 
			{
				Console.WriteLine("{1}\n{2}", e.Message, e.StackTrace);
			}
		}

		public ICollection Apps 
		{
			get { return m_apps.Values; }
		}

		IDomainMasterEvents		m_sink		= null;
		ListDictionary			m_apps		= new ListDictionary();		
		static DomainMaster		m_instance	= null;
		string					m_directory;
	}
}
