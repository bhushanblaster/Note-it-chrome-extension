using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace DocProcessorLib
{
    [Guid("8AEB6C2A-E896-40A6-9BDB-BB97397CDFBD")]
    [InterfaceType(ComInterfaceType.InterfaceIsIDispatch)]
    [ComVisible(true)]
    public interface IDOCProcessor
    {
        bool AddDataToFile(string filepath,string filename, string data,bool isNewFile);
    }

    [Guid("962839C8-D23E-4F94-9082-1FE6AA50D1FC")]
    [ClassInterface(ClassInterfaceType.None)]
    [ComVisible(true)]
    [ProgId("DocProcessorLib.DocProcessor")]
    public class DocProcessor : IDOCProcessor
    {
        public void writeLog(string text)
        {
            string file = @"D:\\docprocessor.txt";
            File.AppendAllText(file, text);
        }
        public bool AddDataToFile(string filepath, string filename, string data, bool isNewFile)
        {
            bool bSuccess = false;
            try
            {
                if (isNewFile)
                {
                    //writeLog("\nWriting new file");
                    Microsoft.Office.Interop.Word._Application oWord;

                    filename = "newfile_" + DateTime.Now.ToString("yyyyMMddHHmmss") + ".docx";
                    string fullfilepath = System.IO.Path.Combine(filepath, filename);

                   // writeLog("fullfilepath = " + fullfilepath);
                    oWord = new Microsoft.Office.Interop.Word.Application();

                    object oMissing = System.Reflection.Missing.Value;
                    Microsoft.Office.Interop.Word._Document oDoc = oWord.Documents.Add(ref oMissing, ref oMissing,
                    ref oMissing, ref oMissing);
                    oWord.ActiveDocument.Characters.Last.Select();  
                    oWord.Selection.Collapse();                      
                    oWord.Selection.TypeText(data);
                    oWord.ActiveDocument.SaveAs2(fullfilepath);
                    oWord.Quit();
                    bSuccess = true;
                }
                else
                {
                   // writeLog("\nWriting existing file");
                    string fullfilepath = filepath + "\\\\" + filename;
                   // writeLog("fullfilepath = " + fullfilepath);
                    Microsoft.Office.Interop.Word._Application oWord;
                    object oMissing = Type.Missing;
                    oWord = new Microsoft.Office.Interop.Word.Application();
                    oWord.Visible = false;
                    oWord.Documents.Open(fullfilepath);
                    oWord.ActiveDocument.Characters.Last.Select();
                    oWord.Selection.Collapse();
                    oWord.Selection.TypeText("\n");
                    oWord.Selection.TypeText(data);
                    oWord.ActiveDocument.Save();
                    oWord.Quit();
                    bSuccess = true;
                }
            }
            catch (Exception ex)
            {
                bSuccess = false;
                //writeLog(ex.ToString());
            }
            return bSuccess;
        }
    }
}
