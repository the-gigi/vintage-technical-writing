using System;

namespace EventDemo
{
	class MainClass
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			Switchboard s = Switchboard.Instance;
	
			EventHandler_1 eh_1 = new EventHandler_1();
			EventHandler_2 eh_2 = new EventHandler_2();
			EventHandler_3 eh_3 = new EventHandler_3();


			s.Subscribe(eh_1 as ISomeInterface);
			s.Subscribe(eh_2 as IAnotherInterface);
			s.Subscribe(eh_3 as ISomeInterface);
			s.Subscribe(eh_3 as IAnotherInterface);

			s.OnThis(5);
			s.OnThat("Yeahh, it works!!!");
			s.OnTheOther(true, "Yeahh, it works!!!");
			s.OnBim();
			s.OnBam();
			s.OnBom();

			s.Unsubscribe(eh_1 as ISomeInterface);
			s.Unsubscribe(eh_2 as IAnotherInterface);
			s.Unsubscribe(eh_3 as ISomeInterface);
			s.Unsubscribe(eh_3 as IAnotherInterface);
		}
	}
}
