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
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;
using System.Globalization;
using System.Collections.ObjectModel;

namespace MP3DurationCalculator
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    public MainWindow()
    {
      InitializeComponent();
      _folderBrowserDialog = new FolderBrowserDialog();
      _folderBrowserDialog.Description =
        "Select the directory that containd the MP3 files.";
      // Do not allow the user to create new files via the FolderBrowserDialog.
      _folderBrowserDialog.ShowNewFolderButton = false;
      _folderBrowserDialog.RootFolder = Environment.SpecialFolder.DesktopDirectory;
      var dt = Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory);
      var start = System.IO.Path.Combine(dt, "Downloads");
      _folderBrowserDialog.SelectedPath = start;
      _results = new ObservableCollection<TrackInfo>();
      files.ItemsSource = _results;
    }

    void collectTrackInfo(string targetFolder)
    {
      btnSelectDir.IsEnabled = false;
      tbTargetFolder.IsEnabled = false;

      var mp3_files = from f in Directory.GetFiles(targetFolder)
                      where System.IO.Path.GetExtension(f) == ".mp3"
                      select f;

      mp3_files = mp3_files.Reverse();
      _fileCount = mp3_files.Count();
      _results.Clear();
      if (_fileCount == 0)
      {
        status.Text = "No MP3 files in this folder.";
        btnSelectDir.IsEnabled = true;
        tbTargetFolder.IsEnabled = true;
      }
      else
        _trackDurations = new TrackDurations(mediaElement, mp3_files, onTrackInfo);

    }

    void btnSelectDir_Click(object sender, RoutedEventArgs e)
    {
      DialogResult r = _folderBrowserDialog.ShowDialog();
      if (r == System.Windows.Forms.DialogResult.OK)
      {
        tbTargetFolder.Text = this._folderBrowserDialog.SelectedPath;
      }

      //var mp3_files = from f in Directory.GetFiles(_folderBrowserDialog.SelectedPath)
      //          where System.IO.Path.GetExtension(f) == ".mp3"
      //          select f;


      //_fileCount = mp3_files.Count();
      //_results.Clear();
      //if (_fileCount == 0)
      //{
      //  status.Text = "No MP3 files in this folder.";
      //  btnSelectDir.IsEnabled = true;
      //}
      //else
      //  _trackDurations = new TrackDurations(mediaElement, mp3_files, onTrackInfo);
    }

    void onTrackInfo(TrackInfo ti)
    {
      if (ti == null)
      {
        _maxLength = 0;
        _trackDurations.Dispose();
        status.Text = "Ready.";
        btnSelectDir.IsEnabled = true;
        tbTargetFolder.IsEnabled = true;
      }
      else
      {
        _results.Add(ti);
        // Make sure the new filename fits in the column
        var ft = new FormattedText(
          ti.Name,
          CultureInfo.GetCultureInfo("en-us"),
          System.Windows.FlowDirection.LeftToRight,
          new Typeface(files.FontFamily,
                       files.FontStyle,
                       files.FontWeight,
                       files.FontStretch),
          files.FontSize,
          Brushes.Black);

        if (ft.Width > _maxLength)
        {
          _maxLength = ft.Width;
          var gv = (GridView)files.View;
          var gvc = gv.Columns[0];
          var curWidth = gvc.Width;

          // Reset to a specific width before auto-sizing
          gvc.Width = _maxLength;
          // This causes auto-sizing
          gvc.Width = Double.NaN;
          
        }   

        // Update the status line
        var st = String.Format("Collecting track info {0}/{1} ...",
                                _results.Count,
                                _fileCount);
        status.Text = st;
      }
    }

    private void files_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
      var tp = new TimeSpan();
      foreach (var f in files.SelectedItems)
      {
        tp += ((TrackInfo)f).Duration;      
      }

      var d = new DateTime(tp.Ticks);
      string format = "mm:ss";
      if (tp.Hours > 0)
        format = "hh:mm:ss";

      total.Text = d.ToString(format);
    }

    private void SelectAll_Click(object sender, RoutedEventArgs e)
    {
      files.SelectAll();
    }

    private void UnselectAll_Click(object sender, RoutedEventArgs e)
    {
      files.UnselectAll();
    }

    private void tbTargetFolder_TextChanged(object sender, TextChangedEventArgs e)
    {
      if (Directory.Exists(tbTargetFolder.Text))
        collectTrackInfo(tbTargetFolder.Text);
    } // the length of the longest track name

    FolderBrowserDialog _folderBrowserDialog;
    TrackDurations _trackDurations;
    ObservableCollection<TrackInfo> _results;
    int _fileCount = 0;
    double _maxLength = 0;

  }

  [ValueConversion(typeof(TimeSpan), typeof(string))]
  public class DurationConverter : IValueConverter
  {
    public object Convert(object value, 
                          Type targetType, 
                          object parameter,
                          CultureInfo culture)
    {
      var duration = (TimeSpan)value;
      var d = new DateTime(duration.Ticks);
      string format = "mm:ss";
      if (duration.Hours > 0)
        format = "hh:mm:ss";

      return d.ToString(format);
    }

    public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
    {
        throw new NotImplementedException();
    }
  }

}
