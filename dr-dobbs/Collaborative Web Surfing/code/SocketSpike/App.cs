using System;
using System.Net;
using System.Runtime.InteropServices;

namespace SocketSpike
{
	class App
	{
		[DllImport("msvcrt.dll")]
		static extern int _getch();

		[STAThread]
		static void Main(string[] args)
		{
			int port = 6666;
			TcpEchoServer tes = new TcpEchoServer();
			tes.Listen(port);

			TcpClient tc = new TcpClient();
			IPHostEntry ihe = Dns.Resolve(Dns.GetHostName());
			
			tc.Connect(new IPEndPoint(ihe.AddressList[0], port));

			_getch();	
		}
	}
}
