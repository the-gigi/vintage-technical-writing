using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Diagnostics;

namespace MP3DurationCalculator
{
  /// <summary>
  /// Helper class for binding track durations to the list view
  /// </summary>
  class TrackInfo
  {
    public string Name { get; set; }
    public TimeSpan Duration { get; set; }
  }

  /// <summary>
  /// A delegate that will be called whenever a new track has been opened by the media element
  /// After the last track there will be another call with ti=null
  /// </summary>
  delegate void TrackDurationsDelegate(TrackInfo ti);
  class TrackDurations : IDisposable
  {
    public TrackDurations(
      MediaElement me,
      IEnumerable<string> files, 
      TrackDurationsDelegate d)
    {   
      Debug.Assert(me != null);
      Debug.Assert(d != null);
      _delegate = d;
      _files = new Stack<string>(files);
      Debug.Assert(_files.Count > 0);
      _me = me;
      _me.LoadedBehavior = MediaState.Manual;
      _me.MediaOpened += _onMediaOpened;
      _me.MediaFailed += _onMediaFailed;
      _me.Source = new System.Uri(_files.Peek());
      _me.Play();
    }

    void getNextTrack()
    {
      if (_files.Count == 0)
      {
        _delegate(null);
      }
      else
      {
        // Get the next file
        _me.Source = new System.Uri(_files.Peek());
        _me.Play();
      }
    }

    private void _onMediaOpened(object sender, RoutedEventArgs e)
    {
      _me.Pause();
      Debug.Assert(_me.NaturalDuration.HasTimeSpan);
      TimeSpan duration = _me.NaturalDuration.TimeSpan;
      var ti = new TrackInfo 
      {
        Name = System.IO.Path.GetFileName(_files.Pop()), 
        Duration = duration
      };

      _me.Stop();
      _delegate(ti);

      getNextTrack();

    }

    private void _onMediaFailed(object sender, RoutedEventArgs e)
    {
      // Get rid of the bad file
      _files.Pop();
      getNextTrack();
    }


    #region IDisposable Members

    public void Dispose()
    {
      _me.MediaOpened -= _onMediaOpened;
      _me.MediaFailed -= _onMediaFailed;
    }

    #endregion


    MediaElement _me;
    TrackDurationsDelegate _delegate;
    Stack<string> _files;
  }
}
