using System;

namespace EventDemo
{
	[EventInterface]
	public interface ISomeInterface
	{
		void OnThis(int x);
		void OnThat(string s);
		void OnTheOther(bool b, string s);
	};

	[EventInterface]
	public interface IAnotherInterface
	{
		void OnBim();
		void OnBam();
		void OnBom();
	};
}
