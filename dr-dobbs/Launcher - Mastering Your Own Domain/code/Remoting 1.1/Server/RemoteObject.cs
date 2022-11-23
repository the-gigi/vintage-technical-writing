using System;

namespace R
{
	class RemoteObject : MarshalByRefObject, IRemoteObject
	{
		public RemoteObject()
		{
			Console.WriteLine("RemoteObject Created!!!");			
		}

		public string Name
		{
			get { return "RemoteObject"; }
		}
	}
}
