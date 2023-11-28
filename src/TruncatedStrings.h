static std::string to_string_with_precision(const float a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return std::move(out).str();
}

static std::string AbbreviateValue(const int value)
{
    std::string str;

    //million
    if (value > 1000000)
    {
        str = to_string_with_precision((round(value / 10) * 10) / 1000000, 1);
        str += "M";
    }
    //thousand
    else if (value >= 1000 && value < 1000000)
    {
        str = to_string_with_precision((round(value / 10) * 10) / 1000, 1);
        str += "K";
    }
    //recent tab
    else
    {
        str = std::to_string(value);
    }

    return str;
}