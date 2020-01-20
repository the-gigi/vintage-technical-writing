// Auto-generated Switchboard class
namespace EventDemo
{
	using System;
	using System.Collections;
	using System.Diagnostics;
	
	
	public class Switchboard : EventDemo.ISomeInterface, EventDemo.IAnotherInterface
	{
		
		static Switchboard m_instance = new Switchboard();
		
		private ArrayList m_someInterfaceList = new ArrayList();
		
		private ArrayList m_anotherInterfaceList = new ArrayList();
		
		public static Switchboard Instance
		{
			get
			{
				return m_instance;
			}
		}
		
		public virtual bool Subscribe(EventDemo.ISomeInterface sink)
		{
			if (m_someInterfaceList.Contains(sink))
			{
				Debug.Assert(false);
				return false;
			}
			m_someInterfaceList.Add(sink);
			return true;
		}
		
		public virtual bool Unsubscribe(EventDemo.ISomeInterface sink)
		{
			if (!m_someInterfaceList.Contains(sink))
			{
				Debug.Assert(false);
				return false;
			}
			m_someInterfaceList.Remove(sink);
			return true;
		}
		
		public virtual bool Subscribe(EventDemo.IAnotherInterface sink)
		{
			if (m_anotherInterfaceList.Contains(sink))
			{
				Debug.Assert(false);
				return false;
			}
			m_anotherInterfaceList.Add(sink);
			return true;
		}
		
		public virtual bool Unsubscribe(EventDemo.IAnotherInterface sink)
		{
			if (!m_anotherInterfaceList.Contains(sink))
			{
				Debug.Assert(false);
				return false;
			}
			m_anotherInterfaceList.Remove(sink);
			return true;
		}
		
		// ISomeInterface events
		public virtual void OnThis(int x)
		{
			IEnumerator e = m_someInterfaceList.GetEnumerator();
			for (
			; e.MoveNext(); 
			)
			{
				ISomeInterface sink = e.Current as ISomeInterface;
				sink.OnThis(x);
			}
		}
		
		public virtual void OnThat(string s)
		{
			IEnumerator e = m_someInterfaceList.GetEnumerator();
			for (
			; e.MoveNext(); 
			)
			{
				ISomeInterface sink = e.Current as ISomeInterface;
				sink.OnThat(s);
			}
		}
		
		public virtual void OnTheOther(bool b, string s)
		{
			IEnumerator e = m_someInterfaceList.GetEnumerator();
			for (
			; e.MoveNext(); 
			)
			{
				ISomeInterface sink = e.Current as ISomeInterface;
				sink.OnTheOther(b, s);
			}
		}
		
		// IAnotherInterface events
		public virtual void OnBim()
		{
			IEnumerator e = m_anotherInterfaceList.GetEnumerator();
			for (
			; e.MoveNext(); 
			)
			{
				IAnotherInterface sink = e.Current as IAnotherInterface;
				sink.OnBim();
			}
		}
		
		public virtual void OnBam()
		{
			IEnumerator e = m_anotherInterfaceList.GetEnumerator();
			for (
			; e.MoveNext(); 
			)
			{
				IAnotherInterface sink = e.Current as IAnotherInterface;
				sink.OnBam();
			}
		}
		
		public virtual void OnBom()
		{
			IEnumerator e = m_anotherInterfaceList.GetEnumerator();
			for (
			; e.MoveNext(); 
			)
			{
				IAnotherInterface sink = e.Current as IAnotherInterface;
				sink.OnBom();
			}
		}
	}
}
