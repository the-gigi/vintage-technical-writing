using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// The following assemblies are needed to host IronPython
using IronPython.Hosting;
using Microsoft.Scripting.Hosting;
using Graphr.Properties;
using System.IO;

namespace Graphr
{
  class IronPythonEvaluator
  {
    public IronPythonEvaluator()
    {
      //Creating a new script runtime
      Dictionary<string, object> options = new Dictionary<string, object>();
      options["Debug"] = true;
      var env = Python.CreateEngine(options);

      // Set the default search path for IronPython (not set by default)
      var searchPaths = env.GetSearchPaths();

      // Note: this is hard-coded path to the IronPython 2.7 lib directory
      var p = "C:/Program Files (x86)/IronPython 2.7/Lib";
      if (!Directory.Exists(p))
      {
          p = "C:/Program Files/IronPython 2.6 for .NET 4.0/Lib";
      }

      searchPaths.Add(p);
      env.SetSearchPaths(searchPaths);
      var filename =  Path.GetTempPath() + "Evaluatr.py";
      // Write the Evaluatr.py script (loaded from resource)
      using (var fs = new FileStream(filename, FileMode.Create, FileAccess.ReadWrite))
      {
        BinaryWriter bw = new BinaryWriter(fs);
        bw.Write(Resources.Evaluatr);      
      }

      evaluatr = env.Runtime.UseFile(filename);

      // Delete the file
      File.Delete(filename);

    }

    ~IronPythonEvaluator()
    {
    }

    public IList<KeyValuePair<double, double>> evaluate(string expr, IEnumerable<double> values)
    {
      var result = new List<KeyValuePair<double, double>>();

      //Invoke the method
      var r = evaluatr.evaluate(expr, values);

        
      foreach (var s in r)
      {
        try
        {
          double x = s[0];
          double y = s[1];
          var pair = new KeyValuePair<double, double>(x, y);
          result.Add(pair);
        }
        catch (Exception)
        {
          throw new Exception("Invalid expression: '" + expr + "'");
        }
      }

      return result;
    }

    public IList<KeyValuePair<double, double>> evaluate(string expr, IEnumerable<int> values)
    {
      // Convert integers to doubles
      var xValues = values.Select(i => (double)i);

      // Call the evaluate() method that operates on doubles
      return evaluatr.evaluate(expr, xValues);
    }

    private dynamic evaluatr = null;
  }

}
