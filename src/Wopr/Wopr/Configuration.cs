using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wopr
{
    public class Configuration
    {
        public static string ServerName
        {
            get
            {
                return System.Configuration.ConfigurationManager.AppSettings.Get("ServerName");
            }
        }

        public static string LobbyAddress
        {
            get
            {
                return System.Configuration.ConfigurationManager.AppSettings.Get("LobbyAddress");
            }
        }

        public static string ArtPath
        {
            get
            {
                return System.Configuration.ConfigurationManager.AppSettings.Get("ArtPath");
            }
        }

        public static string Core
        {
            get
            {
                return System.Configuration.ConfigurationManager.AppSettings.Get("Core");
            }
        }
    }
}
