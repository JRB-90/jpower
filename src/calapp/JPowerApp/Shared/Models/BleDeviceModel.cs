using Plugin.BLE.Abstractions.Contracts;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JPowerApp.Shared.Models
{
    public class BleDeviceModel
    {
        public BleDeviceModel(IDevice device)
        {
            this.device = device;
        }

        public string Name => device.Name;

        private readonly IDevice device;
    }
}
