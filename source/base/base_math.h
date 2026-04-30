#ifndef BASE_MATH_H
#define BASE_MATH_H

internal f32
f(f32 d) {
    return d * d;
}

internal f32
derivative(f32 x, f32 (*f)(f32)) {

  f32 h = 1e-6; // small interval to differnetiate the scope
  return (f(x + h) - f(x - h)) / (2.0 * h);
}


#endif
