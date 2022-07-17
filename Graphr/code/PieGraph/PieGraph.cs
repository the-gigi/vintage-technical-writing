using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Windows;
using System.ComponentModel.Composition;
using System.Diagnostics;

namespace Graphr
{
  [ExportMetadata("Name", "Pie")]
  [Export(typeof(IGraph))]
  class PieGraph : IGraph
  {
    public void PopulateCanvas(
      Canvas c,
      IList<KeyValuePair<double, double>> data,
      IDictionary<string, object> config)
    {

      // Always use the same seed
      var random = new Random(42);

      SolidColorBrush borderBrush;
      try
      {
        borderBrush = new SolidColorBrush((Color)config["BorderColor"]);

        dynamic thickness = config["BorderThickness"];
        this.borderThickness = thickness;



        this.borderBrush = borderBrush;
        this.borderThickness = thickness;
      }
      catch (Exception)
      {
        // Ignore errors in config
        return;
      }

      // Clear the previous graph
      c.Children.Clear();
      int count = data.Count;

      // Convert values to percentages
      var min = (from x in data select x.Value).Min();
      var sum = (from x in data select x.Value - min).Sum();
      var values = from x in data select (x.Value - min) / sum;

      this.radius = 0.45 * Math.Min(c.ActualWidth, c.ActualHeight);

      double startAngle = 0.0;
      Point startPoint = new Point(radius, 0);
      var backToCenter = new LineSegment(new Point(0, 0), true);
      this.center = new Point(c.ActualWidth / 2, c.ActualHeight / 2);
      foreach (var v in values)
      {
        Path path = new Path();

        Canvas.SetLeft(path, c.ActualWidth / 2);
        Canvas.SetTop(path, c.ActualHeight / 2);

        // Create random brush
        var r = (byte)random.Next(256);
        var g = (byte)random.Next(256);
        var b = (byte)random.Next(256);
        
        SolidColorBrush scb = new SolidColorBrush(Color.FromRgb(r, g, b));


        path.Fill = scb;
        path.Stroke = borderBrush;
        path.StrokeThickness = this.borderThickness;

        PathGeometry pathGeometry = new PathGeometry();
        PathFigure pathFigure = new PathFigure();
        pathFigure.StartPoint = startPoint;
        pathFigure.IsClosed = true;

        // Compute angle as percent of the whole circle
        double angle = startAngle + 360 * v;
        var point = new Point(Math.Cos(angle * Math.PI / 180) * radius,
                                 Math.Sin(angle * Math.PI / 180) * radius);

        // Starting Point
        LineSegment lineSegment = new LineSegment(startPoint, true);

        // Arc
        ArcSegment arcSegment = new ArcSegment();
        arcSegment.IsLargeArc = angle - startAngle >= 180.0;
        arcSegment.Point = point;
        arcSegment.Size = new Size(radius, radius);
        arcSegment.SweepDirection = SweepDirection.Clockwise;
        pathFigure.Segments.Add(lineSegment);
        pathFigure.Segments.Add(arcSegment);
        pathFigure.Segments.Add(backToCenter);
        pathGeometry.Figures.Add(pathFigure);
        path.Data = pathGeometry;
        
        startPoint = point;
        startAngle = angle;
        c.Children.Add(path);
      }
    }

    public void DoGraphLayout(Canvas c, IList<KeyValuePair<double, double>> data)
    {
      if (data == null)
        return;

      double radius = 0.45 * Math.Min(c.ActualWidth, c.ActualHeight);
      if (radius == this.radius)
        return;

      double scale = radius / this.radius;
      var t = new ScaleTransform(scale, scale);  

      foreach (var p in c.Children)
      {
        Path path = p as Path;
        if (path != null)
        {
          Canvas.SetLeft(path, c.ActualWidth / 2);
          Canvas.SetTop(path, c.ActualHeight / 2);
          path.RenderTransform = t;

          path.Stroke = this.borderBrush;
          path.StrokeThickness = this.borderThickness;
        }
      }
    }

    public IDictionary<string, Tuple<Type, object>> ConfigSpec
    {
      get
      {
        var d = new Dictionary<string, Tuple<Type, object>>();
        d.Add("BorderColor", Tuple.Create<Type, object>(typeof(Color), Colors.Black));
        d.Add("BorderThickness", Tuple.Create<Type, object>(typeof(double), 2));
        return d;
      }
    }

    Point center;
    double radius;
    SolidColorBrush borderBrush;
    double borderThickness;
  }
}
