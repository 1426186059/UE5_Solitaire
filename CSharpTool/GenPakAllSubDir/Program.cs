namespace GenPakAllSubDir
{
    internal class Program
    {
        static void Main(string[] args)
        {
            List<string> list = new List<string>();
            Console.WriteLine("BaseDirectory: " + AppContext.BaseDirectory);

            string mFileContent = "";
            foreach (var v in Directory.GetDirectories(AppContext.BaseDirectory, "*", SearchOption.AllDirectories))
            {
                string path = v;
                if (Directory.GetFiles(path).Length > 0)
                {
                    mFileContent += path + Path.DirectorySeparatorChar + "*\n";
                }
            }

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
    }
}
