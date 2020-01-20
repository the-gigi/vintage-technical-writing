using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using System.ComponentModel.Composition;

namespace Graphr
{
  [ExportMetadata("Name", "Line")]
  [Export(typeof(IGraph))]
  class LineGraph : IGraph
  {
    public void PopulateCanvas(
     Canvas c,
     IList<KeyValuePair<double, double>> data,
     IDictionary<string, object> config)
    {
      // Clear the previous graph
      c.Children.Clear();
      int count = data.Count;

      var brush = new SolidColorBrush((Color)config["LineColor"]);
      double thickness = 0;

      //var temp = config["StrokeThickness"];
      //if (temp is int)
      //  thickness = (int)temp;
      //else if (temp is uint)
      //  thickness = (uint)temp;
      //else if (temp is float)
      //  thickness = (float)temp;
      dynamic temp = config["StrokeThickness"];
      thickness = temp;


      for (int i = 0; i < count-1; ++i)
      {
        Line line = new Line()
        {
          Stroke = brush,
          StrokeThickness = thickness
        };


        // Add the line to the Canvas.            
        c.Children.Add(line);
      }
    }

    public void DoGraphLayout(Canvas c, IList<KeyValuePair<double, double>> data)
    {
      if (data == null)
        return;

      // Use LINQ query dot syntax to find the max value)
      var maxHeight = (from n in data
                       select n.Value).Max();

      var minHeight = (from n in data
                       select n.Value).Min();

      // Scale all values to fit inside the canvas
      var scale = (c.ActualHeight - 20) / (maxHeight - minHeight);
      List<double> values = new List<double>(from n in data
                                             select n.Value * scale);

      var padding = 10;
      
      var lastValue = values[0];
      var width = (c.ActualWidth - 20) / c.Children.Count;
      var i = 0;
      foreach (Line line in c.Children)
      {
        ++i;
        var x1 = padding / 2 + (i - 1) * width;
        var y1 = (c.ActualHeight - 10) - (lastValue - minHeight * scale);
        var x2 = padding / 2 + i * width;
        var y2 = (c.ActualHeight - 10) - (values[i] - minHeight * scale);
        
        line.X1 = x1;
        line.Y1 = y1;
        line.X2 = x2;
        line.Y2 = y2;

        // update the last value for the next iteration
        lastValue = values[i];
      }
    }

    public IDictionary<string, Tuple<Type, object>> ConfigSpec
    {
      get
      {
        var d = new Dictionary<string, Tuple<Type, object>>();
        d.Add("LineColor", Tuple.Create<Type, object>(typeof(Color), Color.FromRgb(255, 255, 0)));
        d.Add("StrokeThickness", Tuple.Create<Type, object>(typeof(int), 2));
        return d;
      }
    }
  }
}
