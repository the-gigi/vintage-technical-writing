namespace Cosurfer
{  
	using System;
	using System.Xml; 
	using System.Text;
  	using System.IO;
  	using System.Net.Sockets;
	using System.Net;
  	using System.Threading;
	using System.Diagnostics;

	public interface IConnectionManager
	{
		void	Connect(string url);	
		void	Send(string data);
	};
   
  	public interface IConnectionManagerEvents
  	{
		void  OnStartListening(int port);	
		void  OnConnectionAccepted();
		void  OnDisconnect();
		void  OnReceive(string data);
  	};

  	class ConnectionManager : 
		IConnectionManager,
		IDisposable
  	{     	  
    	public ConnectionManager() 
    	{
			ResetPorts();
    	}

		void ResetPorts()
		{
			if (!m_listening)
			{
				m_listenPort	= 8888;
				m_connectPort	= 8889;
			}
			else if (m_listenPort == 8888)
			{
				m_connectPort = 8889;
			} 
			else
			{
				m_connectPort = 8888;
			}

		}

		public void AttachSink(IConnectionManagerEvents sink)
		{
			m_sink = sink;
		}

		public int ListenPort
		{
			get { return m_listenPort;  }
			set { m_listenPort = value; } 
		} 

		/// <summary>
		///	Dispose of resources and suppress finalization
		/// </summary>
		public void Dispose() 
		{
			GC.SuppressFinalize(this);
			CommCleanup();
		}

		public void Connect(string url)
		{
			if (m_stream != null)
				return;

			Console.WriteLine("ConnectionManager::Connect(), url={0}:{1}", url, m_connectPort);

			try
			{
				m_client = new TcpClient();
				m_client.Connect(url, m_connectPort);

				// Get the network stream through which all the reading and writing will occur
				m_stream = m_client.GetStream();   
				m_writer = new StreamWriter(m_stream);
				m_writer.AutoFlush = true;

				AsyncCallback readReadyCallback = new AsyncCallback(OnReadReady);
				m_stream.BeginRead(m_inBuff, 0, BUFF_SIZE, readReadyCallback, this);
			}
			catch (Exception e)
			{
				Console.WriteLine("*** Exception *** ConnectionManager::Connect("+url+":"+m_connectPort+") Failed, Description: " + e.Message);       
			}       
		}

		public void Send(string data)
		{
			if (m_stream == null || m_writer == null)
				return;

			try
			{       				
				m_writer.Write(data);   
			}
			catch (Exception e)
			{
				Console.WriteLine("*** Exception *** ConnectionManager::Send('{0}') Failed, Description: {1}", data, e.Message);        
			}       
		}

		public void Listen()
		{
			try
			{
				CommCleanup();
				ThreadPool.QueueUserWorkItem(new WaitCallback(SockThreadFunc));
			}
			catch (Exception e)
			{
				Console.WriteLine("*** Exception *** ConnectionManager::Listen(), Description: " + e.Message);
			}   
		}

		void CommCleanup()
		{
			if (m_stream != null)
			{
			  	m_stream.Close();
			  	m_stream = null;
			}
		}
        
    	void SockThreadFunc(Object state)
    	{
			Socket listener;               
			listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			listener.Blocking = true;
			IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, m_listenPort);
			try
			{
				listener.Bind(endPoint);
			}
			catch(SocketException e)
			{
				// If there is already a listener on this port, listen on another port
				if(e.ErrorCode == 10048)
				{
					endPoint = new IPEndPoint(IPAddress.Any, ++m_listenPort);
					--m_connectPort;
					listener.Bind(endPoint);
				}
				else
				{
					throw e;
				}
			}
			catch(Exception e)
			{
				Console.WriteLine("Exception - {0}", e.Message);
				Debug.Assert(false);
				return;
			}
			
			m_listening = true;
			m_sink.OnStartListening(m_listenPort);				
			listener.Listen(m_listenPort);
			m_sock = listener.Accept();
			m_listening = false;

			// Notify the sink that a connection was accepted
			m_sink.OnConnectionAccepted();
			
			m_stream = new NetworkStream(m_sock);
			m_writer = new StreamWriter(m_stream);
			m_writer.AutoFlush = true;

			AsyncCallback readReadyCallback = new AsyncCallback(OnReadReady);
			m_stream.BeginRead(m_inBuff, 0, BUFF_SIZE, readReadyCallback, this);			
    	}
    
    	static void OnReadReady(IAsyncResult ar)
    	{  			
     		//Console.WriteLine("ConnectionManager::OnReadReady()");
     		ConnectionManager cm = (ConnectionManager)ar.AsyncState;
     		try
     		{
				int bytes = cm.m_stream.EndRead(ar);
				if (bytes == 0) // Disconnect
				{
					cm.m_sink.OnDisconnect();
					cm.ResetPorts();
					if (!cm.m_listening)
						cm.Listen();
					else
						cm.m_sink.OnStartListening(cm.m_listenPort);
					return;
        		}
        
        		if (bytes > 0)
        		{
					// Extract a string from the bytes array
					string data = Encoding.ASCII.GetString(cm.m_inBuff, 0, bytes);

					cm.m_sink.OnReceive(data);
        		}

        		AsyncCallback readReadyCallback = new AsyncCallback(OnReadReady);
        		cm.m_stream.BeginRead(cm.m_inBuff, 0, BUFF_SIZE, readReadyCallback, cm);
			}
			catch (Exception e)
			{
				Console.WriteLine("*** Excpetion *** ConnectionManager::OnReadReady(), Description: {0} \r\n StackTrace: {1}", e.Message, e.StackTrace);
			}
    	}     

		const int BUFF_SIZE   = 512;        
    	
		byte[]         m_inBuff		= new byte[BUFF_SIZE];  
		byte[]         m_outBuff		= new byte[BUFF_SIZE];
		bool						m_listening     = false;
		int							m_listenPort	= -1;
		int							m_connectPort	= -1;
		Socket						m_sock			= null;
		TcpClient					m_client		= null;
		NetworkStream				m_stream		= null;
		StreamWriter				m_writer		= null;    	
		IConnectionManagerEvents	m_sink = null;

	}
}
