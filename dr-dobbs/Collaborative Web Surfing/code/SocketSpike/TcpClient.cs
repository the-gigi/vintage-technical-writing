using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace SocketSpike
{
	class TcpClient
	{		
		public TcpClient()
		{		
		}

		~TcpClient()
		{		
			Disconnect();
		}

		public void Connect(EndPoint ep)
		{
			m_sock.BeginConnect(ep, new AsyncCallback(StaticConnectCallback), this);
		}

		public void Disconnect()
		{
			m_sock.Close();
		}


		public bool Connected
		{
			get { return m_sock.Connected; } 
		}

		public bool Send(string s)
		{
			if (!m_sock.Connected)
				return false;

			
			int sent = m_sock.Send(Encoding.ASCII.GetBytes(s));

			return sent == s.Length;
		}

		private void OnReceive(IAsyncResult ar)
		{
			int bytes = m_sock.EndReceive(ar);
			string s = Encoding.ASCII.GetString(m_buffer, 0, bytes);
			Console.WriteLine(s);
			
			if (s == "QUIT")
				Disconnect();
			else
				m_sock.BeginReceive(m_buffer, 0, BUFF_SIZE, SocketFlags.None, new AsyncCallback(StaticReceiveCallback), this);
		}

		private void OnConnect(IAsyncResult ar)
		{
			m_sock.EndConnect(ar);
			m_sock.BeginReceive(m_buffer, 0, BUFF_SIZE, SocketFlags.None, new AsyncCallback(StaticReceiveCallback), this);
		}

		public static void StaticConnectCallback(IAsyncResult ar) 
		{
			TcpClient tes = ar.AsyncState as TcpClient;
			tes.OnConnect(ar);
		}

		public static void StaticReceiveCallback(IAsyncResult ar) 
		{
			TcpClient tes = ar.AsyncState as TcpClient;
			tes.OnReceive(ar);
		}

		const int BUFF_SIZE = 512;            	
		Socket    m_sock    = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		byte[]    m_buffer	= new byte[BUFF_SIZE];  
	}
}

//		try
//		{
//			
//		}
//	catch(SocketException e)
//	{
//		// If there is already a listener on this port, listen on another port
//		if(e.ErrorCode == 10048)
//		{
//		endPoint = new IPEndPoint(IPAddress.Any, ++m_listenPort);
//		--m_connectPort;
//		listener.Bind(endPoint);
//		}
//		else
//		{
//			throw e;
//		}

//	catch(Exception e)
//{
//	Console.WriteLine("Exception - {0}", e.Message);
//	Debug.Assert(false);
//	return;
//}
//			
//	m_listening = true;
//	m_sink.OnStartListening(m_listenPort);				
//	listener.Listen(m_listenPort);
//	m_sock = listener.Accept();z
//	m_listening = false;
//
//	// Notify the sink that a connection was accepted
//	m_sink.OnConnectionAccepted();
//			
//	m_stream = new NetworkStream(m_sock);
//	m_writer = new StreamWriter(m_stream);
//	m_writer.AutoFlush = true;
//
//	AsyncCallback readReadyCallback = new AsyncCallback(OnReadReady);
//	m_stream.BeginRead(m_inBuff, 0, BUFF_SIZE, readReadyCallback, this);			

/*
public class AsynchronousSocketListener {
    
    // Incoming data from client.
    public static string data = null;

    // Thread signal.
    public static ManualResetEvent allDone = new ManualResetEvent(false);

    public AsynchronousSocketListener() {
    }

    public static void StartListening() {
        // Data buffer for incoming data.
        byte[] bytes = new Byte[1024];

        // Establish the local endpoint for the socket.
        // The DNS name of the computer
        // running the listener is "host.contoso.com".
        IPHostEntry ipHostInfo = Dns.Resolve(Dns.GetHostName());
        IPAddress ipAddress = ipHostInfo.AddressList[0];
        IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 11000);

        // Create a TCP/IP socket.
        Socket listener = new Socket(AddressFamily.InterNetwork,
            SocketType.Stream, ProtocolType.Tcp );

        // Bind the socket to the local endpoint and listen for incoming connections.
        try {
            listener.Bind(localEndPoint);
            listener.Listen(100);

            while (true) {
                // Set the event to nonsignaled state.
                allDone.Reset();

                // Start an asynchronous socket to listen for connections.
                Console.WriteLine("Waiting for a connection...");
                listener.BeginAccept( 
                    new AsyncCallback(AcceptCallback),
                    listener );

                // Wait until a connection is made before continuing.
                allDone.WaitOne();
            }

        } catch (Exception e) {
            Console.WriteLine(e.ToString());
        }

        Console.WriteLine("\nPress ENTER to continue...");
        Console.Read();
        
    }

    public static void AcceptCallback(IAsyncResult ar) {
        // Signal the main thread to continue.
        allDone.Set();

        // Get the socket that handles the client request.
        Socket listener = (Socket) ar.AsyncState;
        Socket handler = listener.EndAccept(ar);

        // Create the state object.
        StateObject state = new StateObject();
        state.workSocket = handler;
        handler.BeginReceive( state.buffer, 0, StateObject.BufferSize, 0,
            new AsyncCallback(ReadCallback), state);
    }

    public static void ReadCallback(IAsyncResult ar) {
        String content = String.Empty;
        
        // Retrieve the state object and the handler socket
        // from the asynchronous state object.
        StateObject state = (StateObject) ar.AsyncState;
        Socket handler = state.workSocket;

        // Read data from the client socket. 
        int bytesRead = handler.EndReceive(ar);

        if (bytesRead > 0) {
            // There  might be more data, so store the data received so far.
            state.sb.Append(Encoding.ASCII.GetString(
                state.buffer,0,bytesRead));

            // Check for end-of-file tag. If it is not there, read 
            // more data.
            content = state.sb.ToString();
            if (content.IndexOf("<EOF>") > -1) {
                // All the data has been read from the 
                // client. Display it on the console.
                Console.WriteLine("Read {0} bytes from socket. \n Data : {1}",
                    content.Length, content );
                // Echo the data back to the client.
                Send(handler, content);
            } else {
                // Not all data received. Get more.
                handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0,
                new AsyncCallback(ReadCallback), state);
            }
        }
    }
    
    private static void Send(Socket handler, String data) {
        // Convert the string data to byte data using ASCII encoding.
        byte[] byteData = Encoding.ASCII.GetBytes(data);

        // Begin sending the data to the remote device.
        handler.BeginSend(byteData, 0, byteData.Length, 0,
            new AsyncCallback(SendCallback), handler);
    }

    private static void SendCallback(IAsyncResult ar) {
        try {
            // Retrieve the socket from the state object.
            Socket handler = (Socket) ar.AsyncState;

            // Complete sending the data to the remote device.
            int bytesSent = handler.EndSend(ar);
            Console.WriteLine("Sent {0} bytes to client.", bytesSent);

            handler.Shutdown(SocketShutdown.Both);
            handler.Close();

        } catch (Exception e) {
            Console.WriteLine(e.ToString());
        }
    }


    public static int Main(String[] args) {
        StartListening();
        return 0;
    }
}
*/ 


