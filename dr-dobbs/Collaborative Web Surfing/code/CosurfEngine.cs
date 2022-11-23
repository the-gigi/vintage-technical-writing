// CosurfEngine.cs
namespace Cosurfer
{  
	using System;
	using System.Xml; 
	using System.Text;
  	using System.IO;
    using SHDocVw;
  	using mshtml;
	using System.Diagnostics;
   
  	public interface ICosurfEngineEvents
  	{
  		void  OnStartListening(CosurfEngine engine, int port);	
   		void  OnConnectionAccepted(CosurfEngine engine);
    	void  OnDisconnect(CosurfEngine engine);
    	void  OnIncomingChatLine(CosurfEngine engine, string chatLine);
  	};

  	/// <summary>
  	///    This class is responsible for managing the entire Cosurfing activity
  	/// </summary>
  	public class CosurfEngine : 
		IDisposable,
		IConnectionManagerEvents,
		IBrowserEvents
  	{
    	private enum State
    	{
      		Idle,
      		Updating
    	};
  
		IConnectionManager	m_connectionManager = null;
		Browser				m_browser		= null;
		XmlNode				m_surfBuffer	= null;
		State				m_state			= State.Idle;
		StringBuilder		m_partialBuffer = new StringBuilder();
    	
		private ICosurfEngineEvents m_sink;
   		  
    	public CosurfEngine(IConnectionManager cm) 
    	{
			m_connectionManager = cm;
    	}

		public void AttachSink(ICosurfEngineEvents sink)
		{
			m_sink = sink;
		}
        
		/// <summary>
		///	Dispose of resources and suppress finalization
		/// </summary>
    	public void Dispose() 
    	{
			GC.SuppressFinalize(this);
			if (m_browser != null)
				m_browser.Dispose();
    	}

		public void Connect(string url)
		{
			m_connectionManager.Connect(url);
		}

		public void OnDocumentComplete(Browser sender, IHTMLDocument2 doc)
    	{   
    		if (doc == null)
    			return;
    			
			if (!m_browser.Busy)
				m_state = State.Idle;

			try
			{
				XmlNode newBuffer = Doc2Buffer(doc);
				string state = System.Enum.GetName(m_state.GetType(), m_state);
				Console.WriteLine(" ---- OnDocumentComplete(url = {0}, m_state = {1}) ----", doc.url, state);
				DumpBuffer(newBuffer, null);
				Console.WriteLine(" ------------------------------");					
									
				bool bSameBuffer = IsSameSurfBuffer(m_surfBuffer, newBuffer);
				if (m_state == State.Idle && !bSameBuffer)
				{
					m_surfBuffer = newBuffer;
					SendSurfBuffer(newBuffer.OwnerDocument.InnerXml);
				}
				else if (m_state == State.Updating)
				{
					if (bSameBuffer)
					{
						m_state = State.Idle;
					}
				}
			}
			catch (Exception e)
			{
				Console.WriteLine("*** Exception *** CosurfEngine::OnDocumentComplete(), Description: " + e.Message);       
			}       
    	}
  		
  		private void DumpBuffer(XmlNode node, string sWhitespace)
  		{
  			if (node == null)
  				return;
  			 	
  			if (node.Attributes != null && node.Attributes.GetNamedItem("Url") != null)
			{
  				Console.WriteLine(sWhitespace + node.Attributes.GetNamedItem("Url").Value);			
  			}
  			
  			sWhitespace = "  " + sWhitespace;
  			foreach (XmlNode child in node.ChildNodes)
  			{
  				DumpBuffer(child, sWhitespace);
  			}
  		}
  
      /// <summary>
      ///    This method updates the browser's document according to current buffer.
      ///    It returns true if any URL was actually modified, false otherwise
      /// </summary>
		private bool Buffer2Doc(IHTMLDocument2 doc, XmlNode buffer)
		{
			try
			{
				if (doc == null || buffer == null)
					return false;

			  	string newURL = "";
			  	if (buffer.Attributes != null)
			  		newURL = buffer.Attributes.GetNamedItem("Url").Value;
			  	string currURL  = doc.url; 

			  	if (currURL != newURL)
			  	{
					doc.url = newURL;
				 	return true; 
			  	}
						
        		// Get all HTML elements collection
        		IHTMLElementCollection all = doc.all;
				if (all == null)
					return false;
			
        		// Get the first frame buffer
        		XmlNode frameBuffer = buffer.FirstChild;
							
        		int nElemCount = all.length;
        		
        		// Iterate over all child elements looking for frames
        		for (int i = 0; i < nElemCount; i++)
        		{
        			try
          			{
            			IHTMLElement elem = (IHTMLElement)all.item(i, i);
            			if (elem == null)
            				continue;
	            		
            			if (elem is IWebBrowser2) // frames are represented by IWebBrowser2 elements
            			{
               				IWebBrowser2 browser = (IWebBrowser2)elem;
               				if (browser == null)
               					continue;
               				// Get the document from current browser frame
               				IHTMLDocument2 frameDoc = (IHTMLDocument2)browser.Document;
               				if (frameDoc == null)
               					continue;
               		
               				// Call recursivly to Buffer2Doc with the child frame and corresponding frame buffer
               				bool bUpdate = Buffer2Doc(frameDoc, frameBuffer);
               				if (bUpdate)
               					return true; // return immediately if any update occured
		               		
               				frameBuffer = frameBuffer.NextSibling;
	            		}
		      		}  
			  		catch (Exception e)
          			{
             			Console.WriteLine("*** Exception in CosurfEngine::Buffer2Doc(), \r\nDescription: {0} \r\n StackTrace: {1}", e.Message, e.StackTrace);
          			}              
        		}

			  	return false;
			}
			catch (Exception e)
			{
				Console.WriteLine("*** Exception in CosurfEngine::Buffer2Doc(), \r\nDescription: {0} \r\n StackTrace: {1}", e.Message, e.StackTrace);       
				return false;
			}       
		}
    
		private bool IsSameSurfBuffer(XmlNode node_1, XmlNode node_2)
		{     
			if (node_1 == null && node_2 == null)
				return true;

			bool bNullNode = (node_1 == null) || (node_2 == null);
			if (bNullNode)
			{
				//Console.WriteLine("CosurfEngine::IsSameSurfBuffer(), false 1");
				return false;    
			}

			return (node_1.OwnerDocument.InnerXml == node_2.OwnerDocument.InnerXml);
		}

    
		public void OnQuit(Browser sender)
  		{
  			m_browser = null;
  			Console.WriteLine("OnQuit()");
  		}

		public void SendChatLine(String chatLine)
		{

			m_connectionManager.Send(String.Format("<ChatLine>{0}</ChatLine>", chatLine));
		}   

		private void SendSurfBuffer(String surfBuffer)
		{
			m_sink.OnIncomingChatLine(this, "\r\n\r\n\r\nSendSurfBuffer()");
			m_sink.OnIncomingChatLine(this, surfBuffer);
			m_sink.OnIncomingChatLine(this, "\r\n\r\n\r\n");

			m_connectionManager.Send(surfBuffer);   
		}


    	public void LaunchBrowser()
    	{
			Console.WriteLine("LaunchBrowser()");
			if (m_browser != null)
					return;
					
        	m_browser = new Browser(this); 
    	}
        
		private void OnIncomingSurfBuffer(string xml)
    	{
			Console.WriteLine(xml);
			m_sink.OnIncomingChatLine(this, "\r\n\r\n\r\nOnIncomingsurfBuffer()");
			m_sink.OnIncomingChatLine(this, xml);
			m_sink.OnIncomingChatLine(this, "\r\n\r\n\r\n");
			
    		try
    		{
				// Ignore buffers when updating to prevent perpetuum mobila
				if (m_state == State.Updating && m_browser.Busy)
					return;
				else
					m_state = State.Idle;
				XmlDocument newBufferDoc = new XmlDocument();
				newBufferDoc.LoadXml(xml);

				XmlNode newBuffer = newBufferDoc.DocumentElement.FirstChild;
				if (IsSameSurfBuffer(m_surfBuffer, newBuffer))
					return;

				m_surfBuffer = newBuffer;
				Console.WriteLine(" ---- OnIncomingSurfBuffer(State = {0}) ----", m_state == State.Updating ? "Updating" : "Idle");
				DumpBuffer(m_surfBuffer, null);
				Console.WriteLine(" ------------------------------");	
				
				UpdateBrowser();
			}
			catch (Exception e)
			{
				Console.WriteLine("*** Exception *** CosurfEngine::OnIncomingBuffer() Failed, Description: {0} \r\n StackTrace: {1}", e.Message, e.StackTrace);       
			}
    	}
    
    	/// <summary> 
    	///    This method accepts an IHTMLDocument2 interface reference
    	///    , prepares an XmlTextWriter and calls the recursive method Frame2Xml
    	///    which builds an XML document that contains the frame structure
    	///    of this document. Note that the main document is also considered 
    	///    a frame.
    	/// </summary>
    	private XmlNode Doc2Buffer(IHTMLDocument2 doc)
    	{
        	StringWriter sw = new StringWriter();

        	// Prepare the XML m_writer
        	XmlTextWriter w = new XmlTextWriter(sw);
        	// write namespace names
        	w.Namespaces    = true;
        	w.Formatting    = Formatting.Indented;
        	w.Indentation   = 1;
        	w.IndentChar    = '\t';
        	// use ' for attribute values
        	w.QuoteChar = '\'';

        	// Write the actual XML  
			w.WriteProcessingInstruction("xml", @"version=""1.0""");
        	w.WriteStartElement("Document");       
        	Frame2Xml(doc, w);
        	w.WriteEndElement();

        	XmlDocument surfBuffer = new XmlDocument();
        	surfBuffer.LoadXml(sw.ToString());

        	return surfBuffer.DocumentElement;
    	}

    	/// <summary> 
    	///    This recursive method builds recursively an XML document
    	///    which contains an hierarchy of feames which represent the frame structure
    	///    of the input HTML document. The URL of each frame is recorded.
    	/// </summary>  
		private void Frame2Xml(IHTMLDocument2 doc, XmlWriter w)
    	{
    		//Console.WriteLine("Frame2Xml(" + doc.url + ")");
			try
			{
				w.WriteStartElement("Frame");
				w.WriteAttributeString("Url", doc.url);

				// Get all HTML elements collection
				IHTMLElementCollection all = doc.all;

				int nElemCount = all.length;
				for (int i = 0; i < nElemCount; i++)
				{
					try
					{
						IHTMLElement elem = (IHTMLElement)all.item(i, i);
						if (elem is IWebBrowser2) // frames are represented by IWebBrowser2 elements
						{
							IWebBrowser2 browser = (IWebBrowser2)elem;

							// Get the document from current browser frame
							IHTMLDocument2 frameDoc = (IHTMLDocument2)browser.Document;
							if (frameDoc != null && frameDoc.url != doc.url)
								Frame2Xml(frameDoc, w);
						}
					}  
					catch (Exception e)
					{
						Console.WriteLine("Exception in Frame2Xml : {0}", e.Message);
					}              
				}

				w.WriteEndElement();        
			}
			catch (Exception e)
			{
				Console.WriteLine("Exception in Frame2Xml : {0}", e.Message);
			} 
    	}
    
		private void UpdateBrowser()
    	{
			try
			{
				Console.WriteLine("CosurfEngine::UpdateBrowser()");
				Console.WriteLine(m_surfBuffer.OuterXml);
				Console.WriteLine(" ------------------------------");	    		

				bool bUpdate = Buffer2Doc(m_browser.Document, m_surfBuffer);
				if (bUpdate)
					m_state = State.Updating;				
			}
			catch (Exception e)
			{
				Console.WriteLine("*** Exception *** CosurfEngine::UpdateBrowser() Failed, Description: " + e.Message);       
			}
    	}
    

		public void  OnStartListening(int port)
		{
			m_sink.OnStartListening(this, port);
		}
		public void  OnConnectionAccepted()
		{
			m_sink.OnConnectionAccepted(this);
		}
		public void  OnDisconnect()
		{
			m_sink.OnDisconnect(this);
		}

		public void OnReceive(string data)
		{
			m_partialBuffer.Append(data);

			int start = m_partialBuffer.ToString().IndexOf(">");
			int end = -1;
			if (m_partialBuffer.ToString().StartsWith("<ChatLine>"))
			{
				end = m_partialBuffer.ToString().IndexOf("</ChatLine>");
				if (end != -1)
				{
					string message = m_partialBuffer.ToString().Substring(start+1, end-start);
					m_sink.OnIncomingChatLine(this, message);
				}
				end += "</ChatLine>".Length;
			}
			else if (m_partialBuffer.ToString().StartsWith("<?xml"))
			{
				end = m_partialBuffer.ToString().IndexOf("</Document>");
				if (end != -1)
				{
					end += "</Document>".Length;
					string buffer = m_partialBuffer.ToString().Substring(0, end);
					OnIncomingSurfBuffer(buffer);
					
				}
			} 
			else
			{
				Debug.Assert(false);
				return;
			}

			if (end > 0)
				m_partialBuffer.Remove(0, end);
		}		
	} 
}
