using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Graphr
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    public MainWindow()
    {
      InitializeComponent();
      this.graphMan = new GraphManager(this.canvas, this.propertyGrid);

      foreach (var h in graphMan.Helpers)
      {
        this.graphSelector.Items.Add(h.Metadata.Name);
      }

      this.graphSelector.SelectedIndex = 0;
      this.graphMan.SwitchGraphHelper((string)this.graphSelector.SelectedValue);
      this.graphSelector.SelectionChanged += onGraphChanged;

      this.ruleTextBox.Text = "sin(x/pi) / cos(x/pi)";
      this.ruleTextBox.Text = "sin(x * pi)";
    }

    private void onGraphChanged(object sender, SelectionChangedEventArgs args)
    {
      graphMan.SwitchGraphHelper((string)this.graphSelector.SelectedValue);
    }

    private void Graphr_SizeChanged(object sender, SizeChangedEventArgs e)
    {
      graphMan.DoGraphLayout();
    }


    private void Graphr_LayoutUpdated(object sender, EventArgs e)
    {
      graphMan.DoGraphLayout();
    }

    
    private void ruleTextBox_TextChanged(object sender, TextChangedEventArgs e)
    {
      enableVisualizeButton();
    }

    private void rangeTextBox_TextChanged(object sender, TextChangedEventArgs e)
    {
      enableVisualizeButton();
    }

    private void enableVisualizeButton()
    {
      if (rangeTextBox == null || ruleTextBox == null)
        return;
      visualizeButton.IsEnabled = (ruleTextBox.Text != "") && (rangeTextBox.Text != "");
    }

    private void visualizeButton_Click(object sender, RoutedEventArgs e)
    {
      var expr = ruleTextBox.Text;
      var values = rangeTextBox.Text.Split(',');
      var start = Convert.ToInt32(values[0]);
      var end = Convert.ToInt32(values[1]);

      graphMan.Update(expr, start, end);
    }

    private GraphManager graphMan = null;
  }
}
