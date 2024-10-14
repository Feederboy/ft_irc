#ifndef EXCEP_HPP
# define EXCEP_HPP

#include <cerrno>
#include <exception>


/*
**  Should create String for add strErrno + Error
*/

class Errfunc : public std::exception
{
    public:
        Errfunc(char *strErrno) : _strErrno(strErrno){}
        virtual const char* what() const throw()
        {
            return (_strErrno);
        }

    private:
        const char * _strErrno;
};


class ErrGetAddrInfo : public std::exception
{
    public:
        virtual const char* what() const throw()
        {
            return ("Error: Getaddrinfo Failed\n");
        }
};

class ErrSocket : public std::exception
{
    public:
        ErrSocket(void) : _strErrno(NULL) {};
        ErrSocket(const char* str) : _strErrno(str) {};
        virtual const char* what() const throw()
        {
            if (!_strErrno)
                return ("Error: Socket failed\n");
            else
                return(_strErrno);
        }
    private:
        const char* _strErrno;
};

class ErrFcntl : public std::exception
{
    public:
        virtual const char* what() const throw()
        {
            return ("Error: Fcntl failed\n");
        }
};

class ErrBind : public std::exception
{
    public:
        virtual const char* what() const throw()
        {
            return ("Error: Bind failed\n");
        }
};

class ErrListen : public std::exception
{
    public:
        virtual const char* what() const throw()
        {
            return ("Error: Listen failed\n");
        }
};

class ErrAccept : public std::exception
{
    public:
        ErrAccept(void) : _strErrno(NULL) {};
        ErrAccept(const char* str) : _strErrno(str) {};
        virtual const char* what() const throw()
        {
            if (!_strErrno)
                return ("Error: Accept failed\n");
            else
                return(_strErrno);
        }
    private:
        const char* _strErrno;
};


class ErrRecv : public std::exception
{
    public:
        ErrRecv(void) : _strErrno(NULL) {};
        ErrRecv(const char* str) : _strErrno(str) {};
        virtual const char* what() const throw()
        {
            if (!_strErrno)
                return ("Error: Recv failed\n");
            else
                return(_strErrno);
        }
    private:
        const char* _strErrno;
};

class ErrPoll : public std::exception
{
    public:
        ErrPoll(void) : _strErrno(NULL) {};
        ErrPoll(const char* str) : _strErrno(str) {};
        virtual const char* what() const throw()
        {
            if (!_strErrno)
                return ("Error: Poll failed\n");
            else
                return(_strErrno);
        }
    private:
        const char* _strErrno;
};

class ControlC : public std::exception
{
    public:
        virtual const char* what() const throw()
        {
            return ("Quiting the program after a ctrl + C\n");
        }
};


#endif