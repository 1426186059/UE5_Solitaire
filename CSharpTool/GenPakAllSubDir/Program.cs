using System.IO;

namespace GenPakAllSubDir
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("BaseDirectory: " + AppContext.BaseDirectory);

            List<string> list = new List<string>();
            foreach (var v in Directory.GetDirectories(AppContext.BaseDirectory, "*", SearchOption.AllDirectories))
            {
                if (Directory.GetFiles(v).Length > 0)
                {
                    list.Add(v);
                }
            }

            list.Sort((x, y) =>
            {
                return -(x.Length - y.Length);
            });

            string mFileContent = "";
            foreach (var path in list)
            {
                mFileContent += path + Path.DirectorySeparatorChar + "*\n";
            }
            mFileContent += GetContentDirEmptyAsset();

            File.WriteAllText(Path.Combine(AppContext.BaseDirectory, "pak_dir_list.txt"), mFileContent);

            Console.WriteLine("--------- Finish ---------");
            while (true)
            {
                if (Console.KeyAvailable)
                {
                    Console.WriteLine("检测到按键，退出。");
                    break;
                }
            }
        }

        static string GetContentDirEmptyAsset()
        {
            string content1 = $"{Path.DirectorySeparatorChar}Content{Path.DirectorySeparatorChar}";
            if (AppContext.BaseDirectory.Contains(content1))
            {
                int nIndex = AppContext.BaseDirectory.IndexOf(content1);
                string contentDir = AppContext.BaseDirectory.Substring(0, nIndex + content1.Length);
                return Path.Combine(contentDir, "============================================.empty"); 
            }
            return "";
        }
    }
}
