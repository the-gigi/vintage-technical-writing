using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections;

namespace SocketSpike
{
	class TcpClient
	{		
		public TcpClient()
		{		
			m_stack.Push("QUIT");
			m_stack.Push("PRETTY GOOD");
			m_stack.Push("HOW ARE YOU?");
			m_stack.Push("HELLO");
		}

		~TcpClient()
		{		
			Disconnect();
		}

		public void Connect(EndPoint ep)
		{
			m_sock.BeginConnect(ep, new AsyncCallback(OnConnect), this);
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
			Console.WriteLine("Client received: {0}", s);

			if (m_stack.Count == 0)
				Disconnect();
			else
			{
				Send(m_stack.Pop() as string);
				m_sock.BeginReceive(m_buffer, 0, BUFF_SIZE, SocketFlags.None, new AsyncCallback(OnReceive), this);
			}
		}

		private void OnConnect(IAsyncResult ar)
		{
			m_sock.EndConnect(ar);
			Send(m_stack.Pop() as string);
			m_sock.BeginReceive(m_buffer, 0, BUFF_SIZE, SocketFlags.None, new AsyncCallback(OnReceive), this);
		}

		const int BUFF_SIZE = 512;            	
		Socket    m_sock    = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		byte[]    m_buffer	= new byte[BUFF_SIZE];  
		Stack	  m_stack	= new Stack();
	}
}
