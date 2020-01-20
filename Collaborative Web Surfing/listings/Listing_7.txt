using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace SocketSpike
{
	class TcpEchoServer
	{
		public void Listen(int port)
		{
			m_listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			m_listener.Blocking = true;
			IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, port);
			m_listener.Bind(endPoint);
			m_listener.Listen(1);
			m_listener.BeginAccept(new AsyncCallback(OnIncomingConnection), null);
		}

		~TcpEchoServer()
		{
			if (m_listener != null)
				m_listener.Close();
			if (m_sock != null)
				m_sock.Close();
		}

		private void OnReceive(IAsyncResult ar)
		{
			int bytes = m_sock.EndReceive(ar);
			if (bytes == 0) // connection closed
			{
				m_sock.Close();
				m_listener.Close();
				return;
			}
			string s = Encoding.ASCII.GetString(m_buffer, 0, bytes);
			Console.WriteLine("Server received: {0}", s);
			
			m_sock.Send(m_buffer, bytes, SocketFlags.None);
			m_sock.BeginReceive(m_buffer, 
				0, 
				BUFF_SIZE, 
				0,
				new AsyncCallback(OnReceive), 
				this);
		}

		private void OnIncomingConnection(IAsyncResult ar)
		{
			m_sock = m_listener.EndAccept(ar);
			m_listener.Close();
			m_sock.BeginReceive(m_buffer, 
								 0, 
				                 BUFF_SIZE, 
				                 0,
				                 new AsyncCallback(OnReceive), 
				                 this);
		}


		Socket    m_listener = null; // Listens for incoming connections
		Socket    m_sock     = null; // An accepted connection to send ECHO responses
		const int BUFF_SIZE  = 512;            	
		byte[]    m_buffer	 = new byte[BUFF_SIZE];
	}
}