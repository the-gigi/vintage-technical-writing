
namespace Cosurfer
{
	using System;
  	using System.Runtime.InteropServices;
	using SHDocVw;
	using mshtml;
	using System.Diagnostics;

	public interface IBrowserEvents
	{
		void OnDocumentComplete(Browser sender, IHTMLDocument2 doc);
		void OnQuit(Browser sender);
	}

  	public class Browser
  	{     
    	/// <summary> 
    	///    This property returns the internal IHTMLDocument2 interface
    	///    of the BrowserControl. If something is wrong ( m_browser not initialized,
    	///    BrowserControl is busy or the document itself is not complete/interactive) 
    	///	 it returns null.
    	/// </summary>   
    	public IHTMLDocument2 Document
    	{
			get 
			{
				try
				{
					IHTMLDocument2 doc = (IHTMLDocument2)m_browser.Document;
					if (doc.readyState == "complete" || doc.readyState == "interactive")
						return doc;
					else	
						return null;
				}
				catch (Exception)
				{
					return null;
				}
			}
    	} 

    	/// <summary> 
    	///    This property returns the busy state of this WebBrowser control
    	/// </summary>   
    	public bool Busy
    	{
      		get { return m_browser == null || m_browser.Busy || Document == null; }
    	} 

    	public Browser(IBrowserEvents sink)
    	{
			InternetExplorer explorer = new SHDocVw.InternetExplorer();
			if (explorer == null || sink == null)
			{
				Debug.Assert(false);
				return;
			}

			AttachSink(explorer, sink);

			// go to home page
			m_browser = (IWebBrowserApp) explorer;
			m_browser.Visible = true;
			m_browser.GoHome();
    	}

    	public void Dispose()
    	{
			if (m_browser != null)
				m_browser.Quit();
    	}
    
    	public void Navigate(String url)
    	{
	      	Object o = null;    
	      	m_browser.Navigate(url, ref o, ref o, ref o, ref o);   
    	}

		void AttachSink(InternetExplorer explorer, IBrowserEvents sink)
		{
			m_sink = sink;

			DWebBrowserEvents2_OnQuitEventHandler qd;
			qd = new DWebBrowserEvents2_OnQuitEventHandler(this.OnQuit);
			explorer.OnQuit += qd;

			DWebBrowserEvents2_DocumentCompleteEventHandler dcd;
			dcd  = new DWebBrowserEvents2_DocumentCompleteEventHandler(this.OnDocumentComplete);
			explorer.DocumentComplete += dcd;

		/*
		  // 2.Fired when browser navigates to a new url
		  DWebBrowserEvents2_NavigateComplete2EventHandler ncd;
		  ncd = new DWebBrowserEvents2_NavigateComplete2EventHandler(this.OnDocumentComplete);
		  explorer.NavigateComplete2 += ncd;
		 
		  //3.Statusbar text changed event
		  DWebBrowserEvents2_StatusTextChangeEventHandler DStatusE
			= new DWebBrowserEvents2_StatusTextChangeEventHandler(OnStatusTextChange);
		  explorer.StatusTextChange += DStatusE;

		  //4.Fired when download progress is updated.
		  DWebBrowserEvents2_ProgressChangeEventHandler DProgressE
			= new DWebBrowserEvents2_ProgressChangeEventHandler(OnProgressChange);
		  explorer.ProgressChange += DProgressE;

		  //5.Command State change event
		  DWebBrowserEvents2_CommandStateChangeEventHandler DCommandE
			= new DWebBrowserEvents2_CommandStateChangeEventHandler(OnCommandStateChange);
		  explorer.CommandStateChange += DCommandE;

		  //6.DownLoad begin event
		  DWebBrowserEvents2_DownloadBeginEventHandler DDownLoadStartE
			= new DWebBrowserEvents2_DownloadBeginEventHandler(OnDownloadBegin);
		  explorer.DownloadBegin += DDownLoadStartE;

		  //7.DownLoad begin event
		  DWebBrowserEvents2_DownloadCompleteEventHandler DDownLoadEndE
			= new DWebBrowserEvents2_DownloadCompleteEventHandler(OnDownloadComplete);
		  explorer.DownloadComplete += DDownLoadEndE;

		  //8.Title Change event
		  DWebBrowserEvents2_TitleChangeEventHandler DTitleChangeE
			= new DWebBrowserEvents2_TitleChangeEventHandler(OnTitleChange);
		  explorer.TitleChange += DTitleChangeE;

		  //9.Property change event
		  DWebBrowserEvents2_PropertyChangeEventHandler DPropertyChangeE
			= new DWebBrowserEvents2_PropertyChangeEventHandler(OnPropertyChange);
		  explorer.PropertyChange += DPropertyChangeE;

		  //10.Before navigate event
		  DWebBrowserEvents2_BeforeNavigate2EventHandler DBeforeNavigateE
			= new DWebBrowserEvents2_BeforeNavigate2EventHandler(OnBeforeNavigate2);
		  explorer.BeforeNavigate2 += DBeforeNavigateE;

		  //12.Creating a new window event
		  DWebBrowserEvents2_NewWindow2EventHandler DNewWindowE
			= new DWebBrowserEvents2_NewWindow2EventHandler(OnNewWindow2);
		  explorer.NewWindow2 += DNewWindowE;

		  //13.Full Screen event
		  DWebBrowserEvents2_OnFullScreenEventHandler DFullScreenE
			= new DWebBrowserEvents2_OnFullScreenEventHandler(OnFullScreen);
		  explorer.OnFullScreen += DFullScreenE;

		  //14.Menubar change event
		  DWebBrowserEvents2_OnMenuBarEventHandler DMenuBarE
			= new DWebBrowserEvents2_OnMenuBarEventHandler(OnMenuBar);
		  explorer.OnMenuBar += DMenuBarE;

		  //15.Toolbar change event
		  DWebBrowserEvents2_OnToolBarEventHandler DToolBarE
			= new DWebBrowserEvents2_OnToolBarEventHandler(OnToolBar);
		  explorer.OnToolBar += DToolBarE;

		  //16.Visibility change event
		  DWebBrowserEvents2_OnVisibleEventHandler DOnVisibleE
			= new DWebBrowserEvents2_OnVisibleEventHandler(OnVisible);
		  explorer.OnVisible += DOnVisibleE;

		  //17.Threat Mode event
		  DWebBrowserEvents2_OnTheaterModeEventHandler DTheaterE
			= new DWebBrowserEvents2_OnTheaterModeEventHandler(OnTheaterMode);
		  explorer.OnTheaterMode += DTheaterE;
		*/        

		   //}

		}

    	///////////////////////////////////////////////////////////////////////////////////////
    	//event handlers are below

    	/// <summary> 
    	///    This event handler is called for two events - 'NavigateComplete'
    	///    and 'DocumentComplete' because the treatment of both events is
    	///    the same.
    	/// </summary>   
		void OnDocumentComplete(Object o, ref Object url)
    	{        
			if (m_sink == null || Document == null)
				return;

			try
			{     		
 				if (!m_browser.Busy)
        			m_sink.OnDocumentComplete(this, this.Document);
			}
				catch (Exception e)
			{
				Console.WriteLine("*** Exception *** Browser::OnDocumentComplete(), Description: " + e.Message);
			}
		}
    
		void OnQuit()
		{
			Console.WriteLine("Internet explorer is quiting");
			if (m_sink != null)
				m_sink.OnQuit(this);
			m_browser  = null;
		}

    void OnStatusTextChange(String sIn)
    {
      Console.WriteLine("Status text changed with {0}", sIn);
    }

    void OnProgressChange(int Progress, int ProgressMax)
    {
      Console.WriteLine("Progress change : progress = {0}, progressMax = {1}", Progress, ProgressMax);
    }

    void OnCommandStateChange(int Command, bool Enable)
    {
      Console.WriteLine("Command State change : Command = {0}, Enable = {1}", Command, Enable);
    }

    void OnDownloadBegin()
    {
      Console.WriteLine("Download begins now");
    }

    void OnDownloadComplete()
    {
      Console.WriteLine("Download completes");
    }

    void OnTitleChange(String Text)
    {
      Console.WriteLine("Title changes to {0}", Text);
    }

    void OnPropertyChange(String property)
    {
      Console.WriteLine("Property {0} changed", property);
    }
  
    void OnBeforeNavigate2(Object o, ref Object URL, ref Object Flags, ref Object TargetFrameName, 
                    ref Object PostData, ref Object Headers, ref bool Cancel)
    {
      Console.WriteLine("Before Navigate2");
    }
      
    void OnNewWindow2(ref Object o, ref bool rb)
    {
      Console.WriteLine("New window");
    }
    
    void OnFullScreen(bool bFullScreen)
    {
      if (bFullScreen)
        Console.WriteLine("Full Screen");
      else
        Console.WriteLine("Not full screen");
    }

    void OnMenuBar(bool b)
    {
      if (b)
        Console.WriteLine("Menu bar visible");
      else
        Console.WriteLine("MenuBar invisible");
    }

    void OnToolBar(bool bVisible)
    {
      if (bVisible)
        Console.WriteLine("Toolbar visible");
      else
        Console.WriteLine("ToolBar invisible");
    }

    void OnVisible(bool bVisible)
    {
      if (bVisible)
        Console.WriteLine("Visible");
      else
        Console.WriteLine("Invisible");
    }

    void OnTheaterMode(bool bTheater)
    {
      if (bTheater)
        Console.WriteLine("Theater Mode");
      else
        Console.WriteLine("Not Threater mode");
    }

    public String Url
      {
          get 
          {
            return m_browser.LocationURL;  
          }
      } 

    private IWebBrowserApp            m_browser  = null;
    private IBrowserEvents            m_sink     = null;
  }
}


