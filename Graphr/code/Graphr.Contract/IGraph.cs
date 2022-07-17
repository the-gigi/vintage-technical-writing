using System;
using System.Collections.Generic;
using System.Windows.Controls;

namespace Graphr
{
  public interface IGraphMetadata
  {
    string Name { get; }
  }

  public interface IGraph
  {
    void PopulateCanvas(
      Canvas c, 
      IList<KeyValuePair<double, double>> data,
      IDictionary<string, object> config);
    
    void DoGraphLayout(
      Canvas c, 
      IList<KeyValuePair<double, double>> data);

    // The config spec is a triplet of config items
    // name: string
    // type: Type
    // initial value: object of the item's type
    IDictionary<string, Tuple<Type, object>> ConfigSpec { get; }
  }
}
