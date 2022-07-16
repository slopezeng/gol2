// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Python.h>
#include "Game.h"
#include "types.h"
#include <stdexcept>
#include <iostream>
#include <vector>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

using namespace std;
using namespace gol;

static Game* s_game = nullptr;

static Cells toCells(PyObject* listofTuples) {
    Cells result;
    if (PyList_Check(listofTuples))
    {
        for (Py_ssize_t i = 0; i < PyList_Size(listofTuples); i++)
        {
            PyObject* innerPair = PyList_GetItem(listofTuples, i);
            vector<LifeInt> coordinate;
            if (PyTuple_Check(innerPair))
            {                
                for (Py_ssize_t j = 0; j < PyTuple_Size(innerPair); j++)
                {
                    PyObject* value = PyTuple_GetItem(innerPair, j);
                    coordinate.push_back(static_cast<LifeInt>(PyFloat_AsDouble(value)));
                }

                if (coordinate.size() != 2)
                {
                    cout << "coord size not 2" << endl;
                    throw logic_error("Passed PyObject pointer was not a list or tuple!");
                }
            }
            else
            {
                cout << "inner pair not tuple" << endl;
                throw logic_error("Passed PyObject pointer was not a list or tuple!");
            }
            Cell c = { coordinate[0], coordinate[1] };
            result.push_back(c);
        }
    }
    else
    {
        cout << "outer list not list" << endl;
        throw logic_error("Passed PyObject pointer was not a list or tuple!");
    }
    return result;
}

PyObject* toListofTuples(const Cells& data) {
    PyObject* listObj = PyList_New(data.size());
    if (!listObj) throw logic_error("Unable to allocate memory for Python list");
    for (unsigned int i = 0; i < data.size(); i++) 
    {
        PyObject* tupleObj = PyTuple_New(2);
        PyObject* x = PyFloat_FromDouble((double)data[i].first);
        PyObject* y = PyFloat_FromDouble((double)data[i].second);
        PyTuple_SET_ITEM(tupleObj, 0, x);
        PyTuple_SET_ITEM(tupleObj, 1, y);
        PyList_SET_ITEM(listObj, i, tupleObj);
    }
    return listObj;
}

PyObject* intialize(PyObject*, PyObject* rhs)
{
    Cells cs = toCells(rhs);
    s_game = new Game(cs);
    return toListofTuples(s_game->CurrentState(false));
}

PyObject* destroy(PyObject*)
{
    delete s_game;
    return PyFloat_FromDouble(0);
}

PyObject* generate(PyObject*)
{
     s_game->Generate(1);
     return toListofTuples(s_game->CurrentState(false));
}


static PyMethodDef golwrapper_methods[] = {
    { "init", (PyCFunction)intialize, METH_O, nullptr },
    { "dest", (PyCFunction)destroy, METH_NOARGS, nullptr },
    { "gen", (PyCFunction)generate, METH_NOARGS, nullptr },

    // Terminator
    { nullptr, nullptr, 0, nullptr }
};


static PyModuleDef golwrapper_module = {
    PyModuleDef_HEAD_INIT,
    "golwrapper", // Module name to use with Python import statements
    "Game of life functions",
    0,
    golwrapper_methods // Structure that defines the methods of the module
};


PyMODINIT_FUNC PyInit_golwrapper() {
    return PyModule_Create(&golwrapper_module);
}

