char buff[18];

void reverse(char *x, int begin, int end)
{
    char c;
    if (begin >= end)
        return;
    c = *(x + begin);
    *(x + begin) = *(x + end);
    *(x + end) = c;
    reverse(x, ++begin, --end);
}
String decimal(String n)
{
    bot.sendMessage("String n : " + n);
    String ans;
    int count = 0;
    for (int i = n.length() - 1; i >= 0; i--)
    {
        ans += n[i];
        count++;
        if (count == 3)
        {
            ans += ('.');
            count = 0;
        }
    }
    if (ans.length() % 4 == 0)
    {
        ans.remove(ans.length() - 1, 1);
    }
    ans.toCharArray(buff, 18);
    reverse(buff, 0, ans.length() - 1);

    bot.sendMessage("Buff : " + String(buff));
    return String(buff);
}

int bytestoKB(int bkb)
{
    return bkb = bkb / 1024;
}

String formatBytes(String s)
{
    int marker = 1024; // Change to 1000 if required
    // int decimal = 3;                                    // Change as required
    int kiloBytes = marker;                             // One Kilobyte is 1024 bytes
    long megaBytes = marker * marker;                   // One MB is 1024 KB
    long gigaBytes = marker * marker * marker;          // One GB is 1024 MB
    long teraBytes = marker * marker * marker * marker; // One TB is 1024 GB

    long bytes = s.toInt();

    // return bytes if less than a KB
    if (bytes < kiloBytes)
        return String(bytes);
    // return KB if less than a MB
    else if (bytes < megaBytes)
        return String(bytes / kiloBytes);
    // return MB if less than a GB
    else if (bytes < gigaBytes)
        return String(bytes / megaBytes);
    // return GB if less than a TB
    else
        return String(bytes / gigaBytes);
}