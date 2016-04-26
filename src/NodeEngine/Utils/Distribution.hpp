#ifndef NDISTRIBUTION_HPP_INCLUDED
#define NDISTRIBUTION_HPP_INCLUDED

#define AURORA_ENABLE_IF(...)  , typename std::enable_if<__VA_ARGS__>::type* = nullptr

#include <functional>
#include <type_traits>

template <typename T>
class Distribution;

namespace detail
{
    template <typename T>
	struct Constant
	{
		explicit Constant(T value) : value(value)
		{
		}

		T operator() () const
		{
			return value;
		}

		T value;
	};

	template <typename Fn, typename T>
	struct IsCompatibleFunction
	{
		static const bool value = !std::is_convertible<Fn, T>::value;
	};

	template <typename U, typename T>
	struct IsCompatibleFunction<Distribution<U>, T>
	{
		static const bool value = std::is_convertible<U, T>::value;
	};
}

/// @brief Class holding a rule to create values with predefined properties
/// @details Contains a callback that returns values on demand. These can be constant (always the same value), according to a
///  random distribution, or be read from a value elsewhere in your code. Generally, the callback can be any function, member
///  function or functor returning a value of type T and taking no arguments.
/// @n@n thor::Distribution can be linked to random distributions of the standard library:
/// @code
/// std::mt19937 engine;
/// std::uniform_int_distribution<int> distr(0, 7);
/// auto randomizer = std::bind(distr, engine);
///
/// thor::Distribution<int> thorDistr(randomizer);
/// @endcode

template <typename T>
class Distribution
{
	private:
		typedef std::function<T()> FactoryFn;

	public:
		template <typename U>
        Distribution(U constant
            AURORA_ENABLE_IF(std::is_convertible<U, T>::value))
		: mFactory(detail::Constant<T>(constant))
		{
		}

		template <typename Fn>
        Distribution(Fn function
            AURORA_ENABLE_IF(detail::IsCompatibleFunction<Fn, T>::value))
		: mFactory(function)
		{
		}

		T operator() () const
		{
			return mFactory();
		}

	private:
		FactoryFn					mFactory;
};

#endif // NDISTRIBUTION_HPP_INCLUDED
