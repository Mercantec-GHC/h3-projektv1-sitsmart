namespace API.Models
{
    public class SitSmartDevice : Common
    {
         
        public required List<TempHumidity> TempHumiditys { get; set; }
        public required List<Movement> Movements { get; set; }
        public required List<DistanceObject> DistanceObjects { get; set; }
    }
}
