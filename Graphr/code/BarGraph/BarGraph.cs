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
  [ExportMetadata("Name", "Bar")]
  [Export(typeof(IGraph))]
  class BarGraph : IGraph
  {
    public void PopulateCanvas(
      Canvas c, 
      IList<KeyValuePair<double, double>> data,
      IDictionary<string, object> config)
    {
      // Clear the previous graph
      c.Children.Clear();
      int count = data.Count;

      // Create brushes to be used by all bars
      var barBrush = new LinearGradientBrush((Color)config["BarColor"], Colors.White, 90.0);
      var strokeBrush = new SolidColorBrush((Color)config["StrokeColor"]);
      
      for (int i = 0; i < count; ++i)
      {
        Rectangle r = new Rectangle();

        // Describes the brush's color using RGB values. 
        // Each value has a range of 0-255.
        byte x = (byte)(i * 255 / count);

        r.Fill = barBrush;
        r.Stroke = strokeBrush;
        //var thickness = config["StrokeThickness"];
        //if (thickness is int)
        //  r.StrokeThickness = (int)thickness;
        //else if (thickness is uint)
        //  r.StrokeThickness = (uint)thickness;
        //else if (thickness is float)
        //  r.StrokeThickness = (float)thickness;
        dynamic thickness = config["StrokeThickness"];
        r.StrokeThickness = thickness;

        // Add the rectangle to the Canvas.            
        c.Children.Add(r);
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

      // Create a space of about 20% between each rectangle
      var elements = data.Count;
      var padding = 10;
      var ratio = 5; // ratio between rectangle width and spacer
      var spacerWidth = Math.Max((c.ActualWidth - padding) / (elements - 1 + elements * ratio), 0);
      int i = 0;
      foreach (Rectangle r in c.Children)
      {
        r.Width = spacerWidth * ratio;
        r.Height = values[i] - minHeight * scale;
        r.SetValue(Canvas.LeftProperty, padding / 2 + i * (r.Width + spacerWidth));
        r.SetValue(Canvas.BottomProperty, 10.0);
        i++;
      }
    }

    public IDictionary<string, Tuple<Type, object>> ConfigSpec
    { 
      get
      {
        var d = new Dictionary<string, Tuple<Type, object>>();
        d.Add("BarColor", Tuple.Create<Type, object>(typeof(Color), Color.FromRgb(255, 0, 255)));
        d.Add("StrokeColor", Tuple.Create<Type, object>(typeof(Color), Colors.Black));
        d.Add("StrokeThickness", Tuple.Create<Type, object>(typeof(int), 2));
        return d;
      }
    }
  }
}
