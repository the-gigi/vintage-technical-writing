using System;

namespace EventDemo
{
	[AttributeUsage( AttributeTargets.Interface, AllowMultiple = false)]
	public class EventInterfaceAttribute : System.Attribute
	{
	}
}
