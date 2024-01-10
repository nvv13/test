
/*
constrain(x, a, b)
Функция проверяет и если надо задает новое значение,
так чтобы оно была в области допустимых значений, заданной параметрами.

Параметры
x: проверяемое значение, любой тип
a: нижняя граница области допустимых значений, любой тип
b: верхняя граница области допустимых значений, любой тип
*/
static int
constrain (int x, int a, int b)
{
  if (x < a)
    return a;
  if (x > b)
    return b;
  return x;
}
static int
random (int min_num, int max_num)
{
  int result = 0, low_num = 0, hi_num = 0;

  if (min_num < max_num)
    {
      low_num = min_num;
      hi_num = max_num + 1; // include max_num in output
    }
  else
    {
      low_num = max_num;
      hi_num = min_num + 1; // include max_num in output
    }

  result = (rand () % (hi_num - low_num)) + low_num;
  result = constrain (result, low_num, hi_num - 1);
  return result;
}
