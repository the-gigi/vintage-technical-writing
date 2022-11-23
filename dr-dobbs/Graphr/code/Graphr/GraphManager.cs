using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using System.IO;

namespace Graphr
{
  class GraphManager
  {
    public GraphManager(Canvas c, Grid g)
    {
      this.canvas = c;
      this.propertyGrid = g;

      // Discover and load graph plugins via MEF magic. All the plugins
      // will automagically populate the Helpers collection


      var catalog = new AggregateCatalog();
      catalog.Catalogs.Add(new DirectoryCatalog(Directory.GetCurrentDirectory()));

      // Find the plugins directory
      var cd = Directory.GetCurrentDirectory();
      var d = cd;
      var pluginsDir = Path.Combine(d, "plugins");
      var root = Directory.GetDirectoryRoot(d);
      while (d != root)
      {
        d = Path.GetDirectoryName(d);
        pluginsDir = Path.Combine(d, "plugins");
        if (Directory.Exists(pluginsDir) && pluginsDir != cd)
        {
          catalog.Catalogs.Add(new DirectoryCatalog(pluginsDir));
          break;
        }
      }
      
      var container = new CompositionContainer(catalog);
      container.ComposeParts(this);
    }

    [ImportMany]
    public IEnumerable<Lazy<IGraph, IGraphMetadata>> Helpers { get; set; }

    public void SwitchGraphHelper(string name)
    {
      this.helper = Helpers.Single(h => h.Metadata.Name == name).Value;

      PopulateGraphProperties(helper.ConfigSpec);
      if (this.data != null)
      {
        PopulateCanvas(this.data);
      }
    }

    private void _onTextChanged(object sender, TextChangedEventArgs eventArgs)
    {
      TextBox tb = sender as TextBox;
      var name = tb.Name;

      config[name] = tb.Text;
      this.Update();
    }

    private void _onIntChanged(object sender, TextChangedEventArgs eventArgs)
    {
      TextBox tb = sender as TextBox;
      var name = tb.Name;

      try
      {
        config[name] = Convert.ToUInt32(tb.Text);
        this.Update();
      }
      catch (Exception)
      {
        // Ignore exceptions here. Exceptions may happen if the user 
        // is in the middle of changing the value.
      }
    }

    private void _onDoubleChanged(object sender, TextChangedEventArgs eventArgs)
    {
      TextBox tb = sender as TextBox;
      var name = tb.Name;

      try
      {
        config[name] = Convert.ToDouble(tb.Text);
        this.Update();
      }
      catch (Exception)
      {
        // Ignore exceptions here. Exceptions may happen if the user 
        // is in the middle of changing the value.
      }
    }

    private void _onColorChanged(object sender, TextChangedEventArgs eventArgs)
    {
      try
      {
        TextBox tb = sender as TextBox;
        var name = tb.Name;

        var a = Convert.ToByte(tb.Text.Substring(1, 2), 16);
        var r = Convert.ToByte(tb.Text.Substring(3, 2), 16);
        var g = Convert.ToByte(tb.Text.Substring(5, 2), 16);
        var b = Convert.ToByte(tb.Text.Substring(7, 2), 16);

        config[name] = Color.FromArgb(a, r, g, b);

        this.Update();
      }
      catch (Exception)
      {
        // Ignore exceptions here. Exceptions may happen if the user 
        // is in the middle of changing the value.
      }
    }

    private UIElement _createEditor(string name, Type t, object v)
    {
      var e = new TextBox() { Name = name, Text = v.ToString() };
      if (t.Name == "string")
        e.TextChanged += new TextChangedEventHandler(_onTextChanged);
      else if (t.Name == "Color")
        e.TextChanged += new TextChangedEventHandler(_onColorChanged);
      else if (t.Name == "Int32")
        e.TextChanged += new TextChangedEventHandler(_onIntChanged);
      else if (t.Name == "Double")
        e.TextChanged += new TextChangedEventHandler(_onDoubleChanged);
      else
      {
        throw new Exception("Unknown type");
      }
      
      return e;
    }

    public void PopulateGraphProperties(IDictionary<string, Tuple<Type, object>> configSpec)
    {
      config = new Dictionary<string, object>();
      foreach (var kv in configSpec)
      {
        config.Add(kv.Key, kv.Value.Item2);
      }

      var g = this.propertyGrid;

      g.ColumnDefinitions.Clear();
      g.RowDefinitions.Clear();
      g.Children.Clear();

      // Define the Columns
      ColumnDefinition colDef1 = new ColumnDefinition();
      ColumnDefinition colDef2 = new ColumnDefinition();
      g.ColumnDefinitions.Add(colDef1);
      g.ColumnDefinitions.Add(colDef2);

      int row = 0;
      foreach (var pair in configSpec)
      {
        var s = pair.Value;
        RowDefinition rd = new RowDefinition();
        rd.Height = new GridLength(25);
        g.RowDefinitions.Add(rd);

        var label = new Label();
        label.Content = pair.Key;
        g.Children.Add(label);
        var editor = _createEditor(pair.Key, s.Item1, s.Item2);
        g.Children.Add(editor);

        Grid.SetRow(label, row);
        Grid.SetColumn(label, 0);

        Grid.SetRow(editor, row);
        Grid.SetColumn(editor, 1);

        ++row;
      }
    }

    public void PopulateCanvas(IList<KeyValuePair<double, double>> data)
    {
      this.data = data;
      this.helper.PopulateCanvas(canvas, data, this.config);
    }

    public void DoGraphLayout()
    {
      if (data != null)
        helper.DoGraphLayout(this.canvas, this.data);
    }

    public void Update(string expr = null, int start = 0, int end = 0)
    {
      var data = this.data;
      if (expr != null)
      {
        var numbers = Enumerable.Range(start, end - start + 1);
        var xValues = numbers.Select(i => (double)i);
        data = evaluator.evaluate(expr, xValues);
      }

      PopulateCanvas(data);
    }

    private Canvas canvas = null;
    private Grid propertyGrid = null;
    private IGraph helper = null;
    private IList<KeyValuePair<double, double>> data;
    private Dictionary<string, object> config;
    private IronPythonEvaluator evaluator = new IronPythonEvaluator();
  }
}
