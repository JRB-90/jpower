using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JPowerApp.Shared.Models
{
    public class BleDeviceModel
    {
        public BleDeviceModel(string name)
        {
            Name = name;
        }

        public string Name { get; }
    }
}
