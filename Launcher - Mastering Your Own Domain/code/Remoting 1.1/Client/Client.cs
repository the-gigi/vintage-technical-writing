using System;
using System.IO;
using System.Diagnostics;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Tcp;
using System.Collections;
using System.Threading;

namespace R
{
	class Client
	{
		[STAThread]
		static void Main(string[] args)
		{
			IObjectProvider op = null;
			try
			{
				// Register a channel using a binary formatter with TypeFilterLevel == Full (to satisfy .NET 1.1 security)
				const int	port = 6100;
				string		uri	= String.Format("tcp://localhost:5700/IObjectProvider", port);

				BinaryServerFormatterSinkProvider provider = new BinaryServerFormatterSinkProvider();
				provider.TypeFilterLevel = System.Runtime.Serialization.Formatters.TypeFilterLevel.Full;

				BinaryClientFormatterSinkProvider clientProv = new BinaryClientFormatterSinkProvider();
				IDictionary props = new Hashtable();
				props["port"] = port;
				TcpChannel channel = new TcpChannel(props, clientProv, provider);        
				ChannelServices.RegisterChannel(channel);

				//
				// Register the JobServerImpl type as a WKO.
				WellKnownClientTypeEntry remoteType = new WellKnownClientTypeEntry(typeof(IObjectProvider), uri);	
				Debug.Assert(remoteType != null);
				RemotingConfiguration.RegisterWellKnownClientType(remoteType);

				op = (IObjectProvider)Activator.GetObject(typeof(IObjectProvider), uri);
				Console.WriteLine("Name = {0}", op.RemoteObject.Name);
				Console.WriteLine("Name = {0}", op.SerialObject.Name);
				Console.WriteLine("Yeahh, it works!!!!");

				while (true)
					Thread.Sleep(100);				
			}
			catch (Exception e)
			{
			
				Console.WriteLine("{0} {1} \n {2}", e.Message, e.StackTrace, e.ToString());
				Console.WriteLine("Swallawed this exception");
			}
			finally
			{
				op = null;
			}

		}
	}
}
