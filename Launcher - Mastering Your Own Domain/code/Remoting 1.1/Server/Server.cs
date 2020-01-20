using System;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Tcp;
using System.Collections;
using System.Diagnostics;
using System.IO;
using System.Threading;

namespace R
{
	class Server
	{
		[STAThread]
		static void Main(string[] args)
		{
			BinaryServerFormatterSinkProvider provider	= null;
			IDictionary props							= null;
			TcpChannel chan								= null;
			try
			{
				RemotingConfiguration.ApplicationName = "Server";
				provider = new BinaryServerFormatterSinkProvider();
				provider.TypeFilterLevel = System.Runtime.Serialization.Formatters.TypeFilterLevel.Full;
				// creating the IDictionary to set the port on the channel instance.
				props = new Hashtable();
				props["port"] = 5700;
				// pass the props for the port setting and the server provider in the server chain argument. (Client remains null here.)
				chan = new TcpChannel(props, null, provider);        
				ChannelServices.RegisterChannel(chan);

				// Register types
				RemotingConfiguration.RegisterWellKnownServiceType(typeof(RemoteObject), 
					"IRemoteObject", 
					WellKnownObjectMode.Singleton);

				RemotingConfiguration.RegisterWellKnownServiceType(typeof(ObjectProvider), 
					"IObjectProvider", 
					WellKnownObjectMode.Singleton);

				Console.WriteLine("Server started. Listening on port {0}", props["port"]);

				// Infinite loop that allows the main thread to be aborted at AppDomain unload
				// Note that Console.Readline() prevents an AppDomain unload
				while (true)
					Thread.Sleep(100);
			}
			catch (Exception e)
			{
				Console.WriteLine("{0} {1} \n {2}", e.Message, e.StackTrace, e.ToString());
				try
				{
					ChannelServices.UnregisterChannel(chan);
					chan = null;
					provider = null;
					props = null;
				}
				catch(Exception ee)
				{
					Console.WriteLine("{0}\n{1}", ee.Message, ee.StackTrace);
				}
				
			}
		}
	}
}
