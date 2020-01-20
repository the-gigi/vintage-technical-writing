using System;

namespace R
{
	[Serializable()]
	public class SerialObject
	{
		public SerialObject()
		{
			Console.WriteLine("SerialObject Created!!!");			
		}

		public string Name
		{
			get { return "SerialObject"; }
		}
	}
}
