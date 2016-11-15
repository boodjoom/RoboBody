#ifndef ERRCODE_H
#define ERRCODE_H

enum ErrCode
{
    ErrOk =0,
    ErrFail =1,
    ErrOpenFail =2,
    ErrWrongCrc =3,
    ErrWrongCode =4,
    ErrWrongState =5,
    ErrWrongData =6,
    ErrSetParamFail =7,
    ErrWriteFail,
    ErrNotImplemented
};

#endif // ERRCODE_H
