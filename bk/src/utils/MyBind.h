#pragma once
namespace myboost
{
	
template < int I >
struct placeholder {};
 
namespace {
placeholder<0> a1;
placeholder<1> a2;
placeholder<2> a3;
}

template < typename ARGS, typename IDX >
IDX lookup(ARGS&, IDX idx) { return idx; }
 
template < typename ARGS, int IDX >
typename boost::fusion::result_of::at< ARGS, boost::mpl::int_<IDX> >::type lookup(ARGS & args, placeholder<IDX>)
{
    return boost::fusion::at_c<IDX>(args);
}

template < int I >
struct invoker {};
 
template <>
struct invoker<0>
{
    template < typename R
             , typename F
             , typename L1
             , typename L2 >
    static R call(F f, L1 & l1, L2 & l2) { return f(); }
};
 
template <>
struct invoker<1>
{
    template < typename R
             , typename F
             , typename L1
             , typename L2 >
    static R call(F f, L1 & l1, L2 & l2)
    {
        return f(lookup(l2, boost::fusion::at_c<0>(l1)));
    }
};
 
template <>
struct invoker<2>
{
    template < typename R
             , typename F
             , typename L1
             , typename L2 >
    static R call(F f, L1 & l1, L2 & l2)
    {
        return f( lookup(l2, boost::fusion::at_c<0>(l1)), lookup(l2, boost::fusion::at_c<1>(l1)) );
    }
};
 
template <>
struct invoker<3>
{
    template < typename R
             , typename F
             , typename L1
             , typename L2 >
    static R call(F f, L1 & l1, L2 & l2)
    {
        return f( lookup(l2, boost::fusion::at_c<0>(l1))
                , lookup(l2, boost::fusion::at_c<1>(l1))
                , lookup(l2, boost::fusion::at_c<2>(l1)) );
    }
};
 
template < typename R
         , typename F
         , typename L1
         , typename L2 >
R invoke(F f, L1 & l1, L2 & l2)
{
    return invoker
           <
             boost::fusion::result_of::size<L1>::type::value
           >::template call<R>(f, l1, l2);
}

template < typename R, typename T >
struct mf0
{
    R operator()(T* t)
    {
        return (t->*f)();
    }
 
    mf0(R(T::*f)()) : f(f) {}
 
private:
    R(T::*f)();
};
template < typename R, typename T, typename A1 >
struct mf1
{
    R operator()(T t, A1 a1) { return (t.*f)(a1); }
 
    mf1(R(T::*f)(A1)) : f(f) {}
 
private:
    R(T::*f)(A1);
};
template < typename R, typename T, typename A1, typename A2 >
struct mf2
{
    R operator()(T t, A1 a1, A2 a2) { return (t.*f)(a1,a2); }
 
    mf2(R(T::*f)(A1,A2)) : f(f) {}
 
private:
    R(T::*f)(A1,A2);
};

template < typename R, typename F, typename L1 >
struct binder_t
{
	binder_t(F f, L1 l1) : mf(f), ml1(l1) {}

	//binder_t(const binder_t<R,F,L1>& src) : mf(src.mf),ml1(src.ml1) {};

    R operator() ()
    {
        boost::fusion::vector<> l2;
        return invoke<R>(mf, ml1, l2);
    }
    /*template < typename A1 >
    R operator()(A1 & a1)
    {
        boost::fusion::vector<A1> l2(a1);
        return invoke<R>(f, l1, l2);
    }
    template < typename A1
             , typename A2 >
    R operator()(A1 a1, A2 a2)
    {
        boost::fusion::vector<A1,A2> l2(a1,a2);
        return invoke<R>(f, l1, l2);
    }
    template < typename A1
             , typename A2
             , typename A3 >
    R operator()(A1 a1, A2 a2, A3 a3)
    {
        boost::fusion::vector<A1,A2,A3> l2(a1,a2,a3);
        return invoke<R>(f, l1, l2);
    }*/  
private:
    F mf;
    L1 ml1;
};

//template < typename F >
//binder_t< typename F::result_type
//        , F
//        , boost::fusion::vector<> >
//bind(F f)
//{
//    typedef typename F::result_type R;
//    typedef boost::fusion::vector<> L1;
//    return binder_t<R, F, L1>(f, L1());
//}
// 
//template < typename F, typename A1 >
//binder_t< typename F::result_type
//        , F
//        , boost::fusion::vector<A1> >
//bind(F f, A1 a1)
//{
//    typedef typename F::result_type R;
//    typedef boost::fusion::vector<A1> L1;
//    return binder_t<R, F, L1>(f, L1(a1));
//}
// 
//template < typename F, typename A1, typename A2 >
//binder_t< typename F::result_type
//        , F
//        , boost::fusion::vector<A1,A2> >
//bind(F f,A1 a1, A2 a2)
//{
//    typedef typename F::result_type R;
//    typedef boost::fusion::vector<A1,A2> L1;
//    return binder_t<R, F, L1>(f, L1(a1,a2));
//}
// 
//template < typename F, typename A1, typename A2, typename A3 >
//binder_t< typename F::result_type
//        , F
//        , boost::fusion::vector<A1,A2,A3> >
//bind(F f, A1 a1, A2 a2, A3 a3)
//{
//    typedef typename F::result_type R;
//    typedef boost::fusion::vector<A1,A2,A3> L1;
//    return binder_t<R, F, L1>(f, L1(a1,a2,a3));
//}

template < typename R
         , typename CLASS, typename OBJ
         , typename DEF >

binder_t< R
        , mf0<R,CLASS>
        , boost::fusion::vector<OBJ> >

//binder_t< R
//        , mf0<R,CLASS>
//        , boost::fusion::vector<OBJ,DEF> >


bind(R(CLASS::*f)(),OBJ a1, DEF a2)
{
    typedef mf0<R,CLASS> F;
    //typedef boost::fusion::vector<OBJ,DEF> L1;

	//template boost::fusion::vector<OBJ>;
	typedef boost::fusion::vector<OBJ> L1;
    //return binder_t<R, F, L1>(F(f), L1(a1,a2));
	return binder_t<R, F, L1>(F(f), L1(a1));
}

}