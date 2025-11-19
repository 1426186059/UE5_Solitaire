namespace CheckPathSame
{
    internal class Program
    {
        static void Main(string[] args)
        {
            List<string> list = new List<string>();
            Console.WriteLine("BaseDirectory: " + AppContext.BaseDirectory);
            foreach (var v in Directory.GetFiles(AppContext.BaseDirectory, "*", SearchOption.AllDirectories))
            {
                string path = v.ToLower();
                if (!list.Contains(path))
                {
                    list.Add(path);
                }
                else
                {
                    Console.WriteLine("重复的文件: " + path);
                    Console.Error.WriteLine("重复的文件: " + path);
                    break;
                }
            }

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
