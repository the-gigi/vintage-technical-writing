// Auto-generated Switchboard class
using System.Collections;
using System.Diagnostics;

namespace EventDemo
{
	class Switchboard :
		EventDemo.ISomeInterface,
		EventDemo.IAnotherInterface
	{
		public static Switchboard Instance
		{
			get { return m_instance; }
		}
		public bool Subscribe(EventDemo.ISomeInterface sink)
		{
			if (m_someInterfaceList.Contains(sink))
			{
				Debug.Assert(false);
				return false;
			}
			m_someInterfaceList.Add(sink);
			return true;
		}

		public bool Unsubscribe(EventDemo.ISomeInterface sink)
		{
			if (!m_someInterfaceList.Contains(sink))
			{
				Debug.Assert(false);
				return false;
			}

			m_someInterfaceList.Remove(sink);
			return true;
		}
		public bool Subscribe(EventDemo.IAnotherInterface sink)
		{
			if (m_anotherInterfaceList.Contains(sink))
			{
				Debug.Assert(false);
				return false;
			}
			m_anotherInterfaceList.Add(sink);
			return true;
		}

		public bool Unsubscribe(EventDemo.IAnotherInterface sink)
		{
			if (!m_anotherInterfaceList.Contains(sink))
			{
				Debug.Assert(false);
				return false;
			}

			m_anotherInterfaceList.Remove(sink);
			return true;
		}

		
		public void OnThis(System.Int32 x)
		{
			foreach (EventDemo.ISomeInterface subscriber in m_someInterfaceList)
				subscriber.OnThis(x);
		}
		
		public void OnThat(System.String s)
		{
			foreach (EventDemo.ISomeInterface subscriber in m_someInterfaceList)
				subscriber.OnThat(s);
		}
		
		public void OnTheOther(System.Boolean b,System.String s)
		{
			foreach (EventDemo.ISomeInterface subscriber in m_someInterfaceList)
				subscriber.OnTheOther(b,s);
		}
		
		public void OnBim()
		{
			foreach (EventDemo.IAnotherInterface subscriber in m_anotherInterfaceList)
				subscriber.OnBim();
		}
		
		public void OnBam()
		{
			foreach (EventDemo.IAnotherInterface subscriber in m_anotherInterfaceList)
				subscriber.OnBam();
		}
		
		public void OnBom()
		{
			foreach (EventDemo.IAnotherInterface subscriber in m_anotherInterfaceList)
				subscriber.OnBom();
		}

		ArrayList m_someInterfaceList = new ArrayList();
		ArrayList m_anotherInterfaceList = new ArrayList();
 
		static Switchboard m_instance = new Switchboard();
	}
}
