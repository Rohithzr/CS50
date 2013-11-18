{

    if (n <= 0)
    {
        return false;
    }
    for (int i = 0; i < n; i++)
    {
        if (array[i] == value)
        {
            return true;
        }
    }
    return false;
}
