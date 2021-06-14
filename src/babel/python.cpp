#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Python.h>

#include <arrow/python/pyarrow.h>
#include <arrow/array/builder_primitive.h>

namespace py = pybind11;
using DoubleArray = arrow::DoubleArray;
using Array = arrow::Array;

double sum(PyObject* source) {
    if(!arrow::py::is_array(source))
        return false;

    arrow::Result<std::shared_ptr<arrow::Array>> result = arrow::py::unwrap_array(source);
    if(!result.ok())
        return false;

    std::shared_ptr<arrow::DoubleArray> a = std::static_pointer_cast<arrow::DoubleArray>(result.ValueOrDie());

    double sum = 0;
    for(int i = 0; i < a->length(); i++) {
        sum += a->Value(i);
    }
    return sum;
}
/*
double sum(std::shared_ptr<arrow::DoubleArray> a) {
    double sum = 0;
    for(int i = 0; i < a->length(); i++) {
        sum += a->Value(i);
    }
    return sum;
}
*/

// for arrow, try something like
/*

   std::shared_ptr<arrow::Buffer> arbitrary_buffer = ... ;
std::shared_ptr<arrow::Buffer> cpu_buffer = arrow::Buffer::ViewOrCopy(
   arbitrary_buffer, arrow::default_cpu_memory_manager());
*/

PYBIND11_MODULE(babel, m) {
  // Ensure dependencies are loaded.
  arrow::py::import_pyarrow();

  m.def("sum", &sum, py::call_guard<py::gil_scoped_release>());
}
