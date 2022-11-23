using System;

namespace EventDemo
{
	class EventHandler_1 : ISomeInterface
	{
		public void OnThis(int x)
		{
			Console.WriteLine("{0} received OnThis({1})", GetType().Name, x);
		}
		public void OnThat(string s)
		{
			Console.WriteLine("{0} received OnThat({1})", GetType().Name, s);
		}

		public void OnTheOther(bool b, string s)
		{
			Console.WriteLine("{0} received OnTheOther({1}, {2})", GetType().Name, b, s);
		}
	}

	class EventHandler_2 : IAnotherInterface
	{
		public void OnBim()
		{
			Console.WriteLine("{0} received OnBim()", GetType().Name);
		}
		public void OnBam()
		{
			Console.WriteLine("{0} received OnBam()", GetType().Name);
		}
		public void OnBom()
		{
			Console.WriteLine("{0} received OnBom()", GetType().Name);
		}
	}

	class EventHandler_3 : ISomeInterface, IAnotherInterface
	{
		public void OnThis(int x)
		{
			Console.WriteLine("{0} received OnThis({1})", GetType().Name, x);
		}
		public void OnThat(string s)
		{
			Console.WriteLine("{0} received OnThat({1})", GetType().Name, s);
		}

		public void OnTheOther(bool b, string s)
		{
			Console.WriteLine("{0} received OnTheOther({1}, {2})", GetType().Name, b, s);
		}

		public void OnBim()
		{
			Console.WriteLine("{0} received OnBim()", GetType().Name);
		}
		public void OnBam()
		{
			Console.WriteLine("{0} received OnBam()", GetType().Name);
		}
		public void OnBom()
		{
			Console.WriteLine("{0} received OnBom()", GetType().Name);
		}
	}

}
