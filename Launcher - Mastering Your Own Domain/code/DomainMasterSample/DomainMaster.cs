using System;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Reflection;

namespace DomainMasterSample
{
	public class AppDomainExplorer : MarshalByRefObject
	{
		public void Explore()
		{
			AppDomain ad = AppDomain.CurrentDomain;
			Console.WriteLine("----- AppDomainExplorer.Explore() -----");
			Console.WriteLine("Name: {0}", ad.FriendlyName);
			Console.WriteLine("Setup Info: {0}", ad.SetupInformation.ToString());
			Console.WriteLine("Assemblies:");
			foreach (Assembly a in ad.GetAssemblies())
			{
				Console.WriteLine(a.FullName);
			}

		}
	}

	class MultiDomainApp
	{
		[LoaderOptimization(LoaderOptimization.MultiDomainHost)]
		static void Main() 
		{
			Console.WriteLine(AppDomain.CurrentDomain.FriendlyName);

			// Create a new AppDoamin
			AppDomain ad = AppDomain.CreateDomain("Playground Domain");			

			// Register domain event handlers
			ad.AssemblyLoad += new AssemblyLoadEventHandler(AssemblyLoadHandler);
			ad.DomainUnload += new EventHandler(DomainUnloadHandler);

			// Exchange data between AppDomains
			ad.SetData("Creator", AppDomain.CurrentDomain);

			// Load and execute assembly in domain
			ad.ExecuteAssembly("Playground.exe");

			// Execute some code in a different AppDomain
			CrossAppDomainDelegate cadd = new CrossAppDomainDelegate(CrossAppDomainCallback);
			ad.DoCallBack(cadd);

			// Create object in another AppDomain
			string path = Path.Combine(Directory.GetCurrentDirectory(), "DomainMasterSample.exe");
			Assembly a = Assembly.LoadFile(path);
			ad.Load(a.GetName());
			AppDomainExplorer ade = ad.CreateInstanceAndUnwrap(a.GetName().FullName, "DomainMasterSample.AppDomainExplorer") as AppDomainExplorer;
			ade.Explore();

			// Unload the domain
			AppDomain.Unload(ad);
			Console.ReadLine();
		}

		static public void AssemblyLoadHandler(object sender, AssemblyLoadEventArgs args) 
		{
			AppDomain ad = sender as AppDomain;
			Debug.Assert(ad == AppDomain.CurrentDomain);
			string a = args.LoadedAssembly.GetName().Name;			
			AppDomain c = ad.GetData("Creator") as AppDomain;
			Console.WriteLine("Assembly Loaded: '{0}' in '{1}' created by '{2}'", a, ad.FriendlyName, c.FriendlyName);
		}

		static public void DomainUnloadHandler(object sender, EventArgs args) 
		{
			AppDomain ad = sender as AppDomain;
			Debug.Assert(ad == AppDomain.CurrentDomain);			
		}

		static public void CrossAppDomainCallback()
		{
			AppDomain ad = AppDomain.CurrentDomain;
			AppDomain c = ad.GetData("Creator") as AppDomain;
			Console.WriteLine("CrossAppDomainCallback() running in '{0}' created by '{1}'", ad.FriendlyName, c.FriendlyName);
		}
	}
}
