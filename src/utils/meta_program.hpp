#pragma once

#include <tuple>
#include <cstddef>

namespace MP
{
	//////////////////////////
	// CONSTANT TRUE/FALSE TYPE
	template<typename T, T VAL>
	struct constant { static constexpr T value{ VAL }; };

	//conceptualizamos el valor ( bool )
	struct true_type : constant<bool,true> {};
	struct false_type : constant<bool,false> {};

	//////////////////////////
	//IS SAME
	template<typename T,typename U>
	struct is_same : false_type{};
	//especialización para int
	template<>
	struct is_same<int,int> : constant<bool,true>{};
	//especialización parcial para cualquier tipo
	template<typename T>
	struct is_same<T,T> : true_type{};
	//constante toma el valor de lo que devuelve la metafuncion is_same
	template<typename T,typename U>
	constexpr bool is_same_v = is_same<T, U>::value;

  ///////////////////////////
  // NTH_TYPE
  template<typename T>
  struct type_id { using type = T; };
  // general
  template<std::size_t N, typename... Ts>
  struct nth_type { static_assert(sizeof...(Ts) != 0, "ERROR: Typelist with 0"); };
  //Helper
  template<std::size_t N, typename... Ts>
  using nth_type_t = typename nth_type<N, Ts...>::type;

  //especializacion para 0, esta requiere que se le pase al menos un tipo
  template<typename T, typename... Ts>
  struct nth_type<0, T, Ts...> : type_id<T> {};
  //especialización para N value, va recursivamente descartando
  // si es 2          <N>  <T> <--Ts------->
  // N = 2 -> nth_type<2, float, int, double >
  // N = 1 -> nth_type<1, int, double>
  // N = 0 -> nth_type<0, double>
  template<std::size_t N, typename T, typename... Ts>
  struct nth_type<N, T, Ts...> : type_id < nth_type_t < N - 1, Ts...> > {};

  ////////////////////////////////
  // POS_TYPE
  template<typename T, typename... Ts>
  struct pos_type { static_assert(sizeof...(Ts) != 0); };

  template<typename T, typename... Ts>
  constexpr std::uint8_t pos_type_v = pos_type<T, Ts...>::value;

  // selecciona esta especializacion si T y T son iguales
  //          T     T   <Ts-------->
  // pos_type<int, int, float, void>
  // se puede acceder al miembro value
  template<typename T, typename... Ts>
  struct pos_type<T, T, Ts...> : constant<std::uint8_t, 0> {};
  // {
  //     onstexpr static std::size_t value { 0 };
  // }
  //caso general
  //                    T      U       <Ts-------->
  // value = 0 pos_type<float, double, int, float>
  //                    T       U    <--Ts---->
  // value = 1 pos_type<float, int, float>
  // value = 2 pos_type<float,float>
  template<typename T, typename U, typename... Ts>
  struct pos_type<T, U, Ts...>
    : constant< std::uint8_t, 1 + pos_type_v<T, Ts...> > {};

  /////////////////////////////////////////////
  //IFT
  template<bool Cond, typename T, typename F>
  struct IFT : type_id < F > {};
  //especializacion
  template<typename T, typename F>
  struct IFT<true, T, F> : type_id < T > {};

  template<bool Cond, typename T, typename F>
  using IFT_t = typename IFT<Cond, T, F>::type;

	//////////////////////////
	//TYPELIST						
  template<typename... Ts> // Ts = TEnemy,TPlayer,TBullet
  struct TypeList {
    //Obtener el número de tipos diferentes pasados por la plantilla
    consteval static std::size_t size() noexcept
    {
      return sizeof...(Ts); 
    }
    template<typename T>
    // Comprueba para cada elemento de Ts si es del tipo T
    // Devuelve false si Ts es vacío
    consteval static bool contains() noexcept {
      return (false || ... || is_same_v<T, Ts>);
    }
    template<typename T>
    consteval static std::uint8_t pos() noexcept {
      static_assert(contains<T>());
      return pos_type_v<T, Ts...>;
    }
  };

  //////////////////////////////////////
  // REPLACE
  // NewType es un tipo que es una plantilla que recibe todo tipo de tipos - plantilla variadica
  template <template <typename...> typename NewType, typename List >
  struct replace {};
  //especializacion parcial
  template <template <typename...> typename NewType, typename ...Ts >
  struct replace<NewType, TypeList<Ts...>> : type_id<NewType<Ts...> > {};
  //HELPER
  template <template <typename...> typename NewType, typename List >
  using replace_t = typename replace<NewType, List>::type;


  // Get the smallest type
  template <typename LIST>
  using smallest_type =
    IFT_t<LIST::size() <= 8, std::uint8_t,
    IFT_t<LIST::size() <= 16, std::uint16_t,
    IFT_t<LIST::size() <= 32, std::uint32_t,
    std::uint64_t>>>;


  // Tuple dispatch
  template<std::size_t I = 0, typename... Tp>
  inline typename std::enable_if<I == sizeof...(Tp), void>::type
    for_each_in_tuple(const std::tuple<Tp...>&, auto)
  { }

  template<std::size_t I = 0, typename... Tp>
  inline typename std::enable_if < I < sizeof...(Tp), void>::type
    for_each_in_tuple(const std::tuple<Tp...>& t, auto f)
  {
    f(std::get<I>(t));
    for_each_in_tuple<I + 1, Tp...>(t, f);
  }


  //////////////////////////////////////
  //CMP TAG TRAITS
  template <typename TLIST>
  struct cmp_tag_traits
  {
    static_assert(TLIST::size() <= 64, "Component tag list is too large");
    // Metafunción para saber el mejor tipo de dato para guardar la máscara (su tamaño)
    using mask_type = smallest_type<TLIST>;

    // Función en tiempo de compilación para saber el tamaño de la lista
    consteval static uint8_t size() noexcept { return TLIST::size(); }

    // Plantilla que nos dice la posición de un tipo (componente o tag) en la lista
    template <typename CMPTAG>
    consteval static uint8_t id() noexcept
    {
      static_assert(TLIST::template contains<CMPTAG>(), "Component or Tag not found");
      return TLIST::template pos<CMPTAG>();
    }

    // Plantilla que nos dice la máscara de un tipo (componente o tag) de la lista
    template <typename... Ts>
    consteval static mask_type mask() noexcept
    {
      return (0 | ... | (1LL << id<Ts>()));
    }
  };

} //namespace MP