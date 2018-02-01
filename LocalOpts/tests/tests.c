int algebraic (int a, int b)
{
    int result = (a/a);

    result *= (b/b);
    result += (b-b);
    result /= result;
    result -= result;
    return result;
}

/*

  result += a * 2; -> result += a<<1;
  result += a * 3;
  result += a * 8; -> result += a<<2;
  result -= b / 2; -> result -= b>>1;
  result -= b / 4; -> result -= b>>2;
  result -= b / 8; -> result -= b>>3;
 */
int strength (int a, int b)
{
    int result = 0;
    result += a * 2;
    result += a * 3;
    result += a * 8;
    result -= b / 2;
    result -= b / 4;
    result -= b / 8;
    return result;
}

int multi_inst (int b)
{
    int result = 0;
    int a = b + 1;
    int c = 4 + a - 1;

    result += a;
    result += b;
    result *= c;
    result /= 2;
    return result;
}

