using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ICSharpCode.USBlib;
using System.Windows.Forms;

namespace LR
{
    /// <summary>Classe para comunicação com o Transmissor Rádio RF12 USB.</summary>
    public partial class TransmissorRFM12USB:System.ComponentModel.Component
    {
        enum Comando :int{ USB_FUNC_CONNECT = 1, USB_FUNC_DISCONNECT, USB_FUNC_TRANSMIT, USB_FUNC_QUERYBUSY, USB_FUNC_QUERYDATA, USB_FUNC_BOOTLOADER, USB_FUNC_SCAN };
        private Bus barramento;
        private Descriptor descritor;
        private Device dispositivo;
        private bool dispositivo_aberto=false;
        /// <summary>String com o Product fornecido pelo dispositivo.</summary>
        public string ProdutoId;
        /// <summary>String com o Manufacturer fornecido pelo dispositivo.</summary>
        public string FabricanteId;

        /// <summary>Procura pelo dispositivo USB e o inicializa para comunicação.
        /// Retorna True se o dispositivo foi encontrado e inicializado, caso contrário, retorna False.
        /// </summary>
        /// <param name="produto">String com o nome do produto retornado pelo Dipositivo USB.</param>
        /// <param name="fabricante">String com o nome do fabricante retornado pelo Dipositivo USB.</param>
        public bool Inicializar(string produto, string fabricante)
        {
            foreach (Bus bus in Bus.Busses)
            {
                foreach (Descriptor descriptor in bus.Descriptors)
                {
                    Device device = descriptor.OpenDevice();
                    if (device.Product == produto && device.Manufacturer == fabricante)
                    {
                        ProdutoId = produto;
                        FabricanteId = fabricante;
                        barramento = bus;
                        descritor = descriptor;
                        dispositivo = device;
                        dispositivo_aberto = true;
                        return true;
                    }
                }
            }
            return false;
        }
        /// <summary>Transmite um array de bytes.
        /// Retorna True se a transmissão foi feita, caso contrário, retorna False.
        /// O dispositivo necessita estar inicializado.
        /// </summary>
        /// <param name="dados">Array de Bytes com os dados a serem transmitidos.</param>
        public bool Transmitir(byte[] dados)
        {
            if (dispositivo_aberto && !Ocupado())
            {
                dispositivo.SendControlMessage(descritor.VendorId & ~(1 << 7), 3, 0, dados);
                return true;
            }
            else
            {
//                throw new Exception("Dispositivo não inicializado.");
                return false;
            }
        }

        /// <summary>Transmite um string.
        /// Retorna True se a transmissão foi feita, caso contrário, retorna False.
        /// O dispositivo necessita estar inicializado.
        /// </summary>
        /// <param name="dados">String com os dados a serem transmitidos.</param>
        public bool Transmitir(string dados)
        {
            byte[] data = new byte[dados.Length];
            for (int i = 0; i < dados.Length; i++)
            {
                data[i] =(byte) dados[i];
            }


            return Transmitir(data);
                
//                System.Text.Encoding.UTF7.GetBytes(dados));
        }

        /// <summary>Verifica o dispositivo está ocupado.
        /// </summary>
        public bool Ocupado()
        {
            if (dispositivo_aberto)
            {
                byte[] dados = new byte[8];
                dispositivo.SendControlMessage(descritor.VendorId | (1 << 7), 4, 0, dados);
                if (dados[0] == 1)
                    return true;
                else
                    return false;
            }
            else
            {
                //                throw new Exception("Dispositivo não inicializado.");
                return false;
            }
        }

        public string Scan()
        {
            if (dispositivo_aberto && !Ocupado())
            {
                string dadoss = string.Empty;
                UTF7Encoding enc = new UTF7Encoding();
                int tamanho=155;
                byte[] dados = new byte[tamanho];
                dispositivo.SendControlMessage(descritor.VendorId | (1 << 7), (int)Comando.USB_FUNC_SCAN, tamanho, dados);
                dadoss += enc.GetString(dados);
                return dadoss;
            }
            else
            {
                return string.Empty;
            }
        }

        /// <summary>Verifica se o dispositivo está presente
        /// </summary>
        public bool Presente()
        {
            if (!dispositivo_aberto)
                return false;

            int c=dispositivo.SendControlMessage(descritor.VendorId | (1 << 7), 4, 0, null);
            if (c < 0)
                return false;
            else
                return true;
        }

        private void InitializeComponent()
        {
            // 
            // TransmissorRFM12USB
            // 

        }

        /// <summary>Recebe dados do dispositivo.
        /// </summary>
        public string Receber()
        {
            if (dispositivo_aberto && !Ocupado())
            {
                string dadoss = string.Empty;
                UTF7Encoding enc = new UTF7Encoding();
                byte[] dados = new byte[8];
                dispositivo.SendControlMessage(descritor.VendorId | (1 << 7), 4, 3, dados);
                int tamanho = dados[1];
                if (tamanho > 0)
                {
                    Array.Resize(ref dados, tamanho);
                    dispositivo.SendControlMessage(descritor.VendorId | (1 << 7), 5, tamanho, dados);
                    dadoss += enc.GetString(dados);
                    return dadoss;
                }
                else
                {
                    //                throw new Exception("Dispositivo não inicializado.");
                    return string.Empty;
                }
            }
            else
            {
                //                throw new Exception("Dispositivo não inicializado.");
                return string.Empty;
            }
        }


    }
}
