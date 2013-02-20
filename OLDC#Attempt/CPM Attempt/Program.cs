using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace CPM_Attempt
{
    class Program
    {
        public static string apath = AppDomain.CurrentDomain.BaseDirectory;

        static void Main(string[] args)
        {
            Console.WriteLine("\n");
            Console.WriteLine("8080 Computer Simulator (C) David Wotherspoon 2012");
            Console.WriteLine("Enter the name of the binary file to load to memory:");
            string file = Console.ReadLine();
            FileStream fs = new FileStream(apath + file, FileMode.Open);
            BinaryReader br = new BinaryReader(fs);
            byte[] mem = br.ReadBytes(65536);
            _8080 i8080 = new _8080(mem);
            i8080.start();
            Console.ReadLine();
        }
    }
}
