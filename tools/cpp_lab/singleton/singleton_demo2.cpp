#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class singleton
{
public:
    static T* get_instance();
private:
    class creator
    {   
        public:
        creator()
        {   
            get_instance();
        }
		inline void dono()const{}
    };  
    static creator instance;
};

template <typename T>
typename singleton<T>::creator singleton<T>::instance;

template <typename T>
T* singleton<T>::get_instance()
{
    static T t;
	instance.dono();
    return &t;
}

#endif
