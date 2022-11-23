using System;
using System.Collections;
using System.Diagnostics;
using System.CodeDom;
using System.CodeDom.Compiler;
using System.IO;
using System.Reflection;
using Microsoft.CSharp;

namespace CodeDOM
{
	/// <summary>
	/// Generates a Switchboard class from a list of interface names
	/// </summary>
	public class SwitchboardGenerator
	{	
		static private void AddEvent(Type i, MethodInfo mi, CodeCommentStatement comment)
		{
			//		public bool OnSomething()
			//		{
			//			foreach (IPlayerActions subscriber in m_playerActionsSubscriberList)
			//				subscriber.OnSomething();
			//		}

			CodeMemberMethod cmm = new CodeMemberMethod();
			if (comment != null)
				cmm.Comments.Add(comment);
			cmm.Name = mi.Name;
			cmm.Attributes = MemberAttributes.Public;

			ParameterInfo[] parameters = mi.GetParameters();
			CodeExpression[] call_parameters = new CodeExpression[parameters.Length];
			int index = 0;
			foreach (ParameterInfo pi in parameters)
			{
				CodeParameterDeclarationExpression cpde = new CodeParameterDeclarationExpression();
				cpde.Name = pi.Name;
				cpde.Type = new CodeTypeReference(pi.ParameterType);
				cmm.Parameters.Add(cpde);
				call_parameters[index++] = new CodeSnippetExpression(pi.Name);
			}

			cmm.ReturnType = new CodeTypeReference(mi.ReturnType);
			
			// IEnumerator enumerator = list.GetEnumerator();
			CodeAssignStatement cas = new CodeAssignStatement();
			cas.Left =  new CodeVariableReferenceExpression("IEnumerator e");
			CodeVariableReferenceExpression cvre = new CodeVariableReferenceExpression(GetListName(i));
			cas.Right = new CodeMethodInvokeExpression(cvre, "GetEnumerator");

			cmm.Statements.Add(cas);

			// Generate a for loop
			CodeIterationStatement loop = new CodeIterationStatement(
												new CodeSnippetStatement(""),
												new CodeMethodInvokeExpression(
													new CodeVariableReferenceExpression("e"),
													"MoveNext"),
													new CodeSnippetStatement(""));
			
			// e.Cuurent as 
			
			loop.Statements.Add(new CodeSnippetStatement(String.Format("{0} sink = e.Current as {0};", i.Name)));
			
			loop.Statements.Add(new CodeMethodInvokeExpression(
									new CodeVariableReferenceExpression("sink"), 
									mi.Name,
									call_parameters));
			
			cmm.Statements.Add(loop);
			m_switchboard.Members.Add(cmm);
		}

		static private string GetListName(Type i)
		{
			return string.Format("m_{0}{1}List",
					i.Name.Substring(1, 1).ToLower(), 
					i.Name.Substring(2, i.Name.Length-2));
		}

		static private void AddStaticInstanceProperty(string ns)
		{
			//	Switchboard m_switchboard = new Switchboard();			
			CodeMemberField field = new CodeMemberField();
			field.Type = new CodeTypeReference("Switchboard");
			field.Name = "m_instance";
			field.Attributes = MemberAttributes.Static;
			field.InitExpression = new CodeSnippetExpression("new Switchboard()");
			m_switchboard.Members.Add(field);

			//	public static Switchboard Instance
			//	{
			//		get
			//		{
			//			return m_instance;
			//		}
			//	}
			CodeMemberProperty cmp = new CodeMemberProperty();
			cmp.Name = "Instance";
			cmp.Attributes = MemberAttributes.Public|MemberAttributes.Static;
			cmp.Type = new CodeTypeReference(String.Format("Switchboard" ,ns));
			cmp.HasGet = true;
			cmp.HasSet = false;			
			cmp.GetStatements.Add(new CodeSnippetStatement("return m_instance;"));			
			m_switchboard.Members.Add(cmp);
		}

		static private void AddSubscribeMethod(Type i)
		{
			//			public bool Subscribe(IPlayerActions sink)
			//			{
			//				if (m_playerActionsSubscriberList.Contains(sink))
			//				{
			//					Debug.Assert(false);
			//					return false;
			//				}
			//				m_playerActionsSubscriberList.Add(sink);
			//				return true;
			//			}
			CodeMemberMethod cmm = new CodeMemberMethod();
			cmm.Name = "Subscribe";
			cmm.Attributes = MemberAttributes.Public;

			CodeParameterDeclarationExpression cpde = new CodeParameterDeclarationExpression();
			cpde.Type = new CodeTypeReference(i);
			cpde.Name = "sink";
			cmm.Parameters.Add(cpde);
			cmm.ReturnType = new CodeTypeReference(Type.GetType("System.Boolean"));
			
			// 'if (list.Contains(sink)) ...' statement
			string listName = GetListName(i);
			CodeConditionStatement ccs = new CodeConditionStatement();
			ccs.Condition = new CodeSnippetExpression(string.Format("{0}.Contains(sink)", listName));

			ccs.TrueStatements.Add(new CodeSnippetStatement("Debug.Assert(false);"));
			ccs.TrueStatements.Add(new CodeSnippetStatement("return false;"));
			cmm.Statements.Add(ccs);
			
			// 'list.Add(sink) statement
			cmm.Statements.Add(new CodeSnippetStatement(string.Format("{0}.Add(sink);", listName)));

			// 'return' statement
			cmm.Statements.Add(new CodeSnippetStatement("return true;"));

			m_switchboard.Members.Add(cmm);
		}

		static private void AddUnsubscribeMethod(Type i)
		{
//		public bool Unsubscribe(IPlayerActions sink)
//		{
//			if (!m_playerActionsSubscriberList.Contains(sink))
//			{
//				Debug.Assert(false);
//				return false;
//			}
//
//			m_playerActionsSubscriberList.Remove(sink);
//
//			return true;
//		}

			CodeMemberMethod cmm = new CodeMemberMethod();
			cmm.Name = "Unsubscribe";
			cmm.Attributes = MemberAttributes.Public;

			CodeParameterDeclarationExpression cpde = new CodeParameterDeclarationExpression();
			cpde.Type = new CodeTypeReference(i);
			cpde.Name = "sink";
			cmm.Parameters.Add(cpde);
			cmm.ReturnType = new CodeTypeReference(Type.GetType("System.Boolean"));
			
			// 'if (!list.Contains(sink)) ...' statement
			string listName = GetListName(i);
			CodeConditionStatement ccs = new CodeConditionStatement();
			ccs.Condition = new CodeSnippetExpression(string.Format("!{0}.Contains(sink)", listName));
			ccs.TrueStatements.Add(new CodeSnippetStatement("Debug.Assert(false);"));
			ccs.TrueStatements.Add(new CodeSnippetStatement("return false;"));
			cmm.Statements.Add(ccs);
			
			// 'list.Remove(sink) statement
			cmm.Statements.Add(new CodeSnippetStatement(string.Format("{0}.Remove(sink);", listName)));

			// 'return' statement
			cmm.Statements.Add(new CodeSnippetStatement("return true;"));

			m_switchboard.Members.Add(cmm);
		}

		static private void AddSinkList(Type i)
		{
			// ArrayList m_someList = new ArrayList();			
			CodeMemberField field = new CodeMemberField();
			field.Type = new CodeTypeReference("ArrayList");
			field.Name = GetListName(i);
			field.InitExpression = new CodeSnippetExpression("new ArrayList()");
			m_switchboard.Members.Add(field);
		}

		static private void AddInterface(Type i)
		{
			// Opening comment
			string comment = String.Format ("{0} events", i.Name);
			CodeCommentStatement c = new CodeCommentStatement(comment);

			ArrayList members = new ArrayList(i.GetMembers()); 
			members.Reverse();
			foreach (MethodInfo mi in members)
			{
				if (members[0] == mi)
					AddEvent(i, mi, c);
				else
					AddEvent(i, mi, null);
			}
		}

		static private void CreateSwitchboard(string ns, ArrayList interfaces)
		{
			// Create namespace
			m_namespace = new CodeNamespace(ns);

			// Create using statements
			m_namespace.Imports.Add (new CodeNamespaceImport ("System") );
			m_namespace.Imports.Add (new CodeNamespaceImport ("System.Collections"));
			m_namespace.Imports.Add (new CodeNamespaceImport ("System.Diagnostics"));

			// Create the Type itself
			m_switchboard = new CodeTypeDeclaration ("Switchboard");
			m_switchboard.TypeAttributes  = TypeAttributes.Public;			
			m_switchboard.IsClass = true;
			m_namespace.Types.Add (m_switchboard);

			// Add base interfaces
			foreach (Type i in interfaces)
				m_switchboard.BaseTypes.Add(new CodeTypeReference(i));
						
			// Add Opening comment
			CodeCommentStatement c = new CodeCommentStatement  (String.Format ("Auto-generated Switchboard class"));
			m_codeGenerator.GenerateCodeFromStatement (c, m_writer, null);
		}

		static private bool GenerateCode() 
		{
			CodeGeneratorOptions cgo = new CodeGeneratorOptions();
			cgo.BlankLinesBetweenMembers = true;
			cgo.BracingStyle = "C";
			cgo.ElseOnClosing = false;
			cgo.IndentString = "\t";
			m_codeGenerator.GenerateCodeFromNamespace(m_namespace, m_writer, cgo);
			m_writer.Close();

			return true;
		}

		public  static bool Generate(string ns, string filename, ArrayList interfaces) 
		{
			if (ns.Length == 0 || interfaces.Count == 0)
			{
				Debug.Assert(false);
				return false;
			}

			m_writer = new StreamWriter (new FileStream (filename, FileMode.Create));
			CreateSwitchboard(ns, interfaces);

			AddStaticInstanceProperty(ns);

			foreach (Type i in interfaces)
			{
				AddSubscribeMethod(i);
				AddUnsubscribeMethod(i);
			}
				
			foreach (Type i in interfaces)
				AddInterface(i);

			foreach (Type i in interfaces)
				AddSinkList(i);

			GenerateCode();
			return true;
		}

		static CodeNamespace		m_namespace		= null;
		static CodeTypeDeclaration m_switchboard	= null;
		static ICodeGenerator		m_codeGenerator = new CSharpCodeProvider().CreateGenerator();
		static TextWriter			m_writer		= null;
	}  	
}
