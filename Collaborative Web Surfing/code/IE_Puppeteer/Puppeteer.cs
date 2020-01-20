using System;
using System.Diagnostics;
using System.Threading;
using SHDocVw;
using mshtml;

namespace IE_Puppeteer
{
	class Puppeteer
	{
		private InternetExplorer m_ie = null;
		private AutoResetEvent m_quitEvent = new AutoResetEvent(false);


		public void OnQuit()
		{
			m_quitEvent.Set();
		}

		public void OnNavigateComplete(Object o, ref Object url)
		{        
			Console.WriteLine("OnNavigateComplete, URL: {0}", m_ie.LocationURL);
		}

		void OnDocumentComplete(Object o, ref Object url)
		{   
			Console.WriteLine("OnDocumentComplete, URL: {0}", url);
			if (m_ie.Busy)
				return;
			IHTMLDocument2 doc = m_ie.Document as IHTMLDocument2;
			DumpDoc(doc, "");
		}

		void DumpDoc(IHTMLDocument2 doc, string indent)
		{
			Debug.Assert(doc != null);
			Console.WriteLine("{0}{1}", indent, doc.url);
			foreach (IHTMLElement e in doc.all)
			{
				if (e is IWebBrowser2)
				{
					IWebBrowser2 wb = e as IWebBrowser2;
					if (!wb.Busy)
						DumpDoc(wb.Document as IHTMLDocument2, indent + "\t");
				}
			}
		}


		Puppeteer()
		{
		}

		void Run()
		{
			m_ie = new InternetExplorer();
			Console.WriteLine("Visible: {0}", m_ie.Visible);
			m_ie.Visible = true;

//			DWebBrowserEvents2_NavigateComplete2EventHandler ncd;
//			ncd = new DWebBrowserEvents2_NavigateComplete2EventHandler(OnNavigateComplete);
//			m_ie.NavigateComplete2 += ncd;

			DWebBrowserEvents2_OnQuitEventHandler qd;
			qd = new DWebBrowserEvents2_OnQuitEventHandler(OnQuit);
			m_ie.OnQuit += qd;

			DWebBrowserEvents2_DocumentCompleteEventHandler dcd;
			dcd  = new DWebBrowserEvents2_DocumentCompleteEventHandler(OnDocumentComplete);
			m_ie.DocumentComplete += dcd;

			Object o = null;  
			string url = System.IO.Path.GetFullPath("../../frameset.html");
			Console.WriteLine("Navigate({0})", url);
			m_ie.Navigate(url, ref o, ref o, ref o, ref o);   

			m_quitEvent.WaitOne();		
		}

		[STAThread]
		static void Main(string[] args)
		{
			new Puppeteer().Run();
		}
	}
}
