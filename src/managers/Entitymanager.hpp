#pragma once

#include "../utils/meta_program.hpp"
#include <cassert>
#include <array>
#include <vector>
#include <optional>
#include <cstdint>
#include <span>
#include <algorithm>
#include <typeinfo>
#include <functional>

// Plantilla para obtener la información de los tags
template <typename TAGS>
struct tag_traits : MP::cmp_tag_traits<TAGS> {};

// Plantilla para obtener la información de los componentes
template <typename CMPS>
struct cmp_traits : tag_traits<CMPS> {};

namespace ETMG
{
	template<typename CMPList,typename TAGList, typename SNGLIst>//typename SNGLIst
	struct EntityManager
	{
		static constexpr std::size_t MAX_ENTITIES{ 50 }; //máximo de entidades vivas

		inline static std::size_t nextID{ 0 }; // ID de la siguiente entidad que se crea

		using tag_info = tag_traits<TAGList>; // Alias para la información de los tags
		using cmp_info = cmp_traits<CMPList>; // Alias para la información de los componentes

		using mask_type_cmp = typename cmp_info::mask_type;
		using mask_type_tag = typename tag_info::mask_type;

		// Alias para reemplazar directamente un TypeList por una tupla
		template <typename List>
		using tuple_replace = MP::replace_t<std::tuple, List>;

		// tipo para Tupla de componentes singlenton
		using singlestorage_t = MP::replace_t<std::tuple, SNGLIst>;

		struct Entity
		{
		friend struct EntityManager<CMPList,TAGList,SNGLIst>; // SNGLIst

		Entity() { id_ = nextID++; printf("Creo Entidad\n"); }
		public:
			// Asigna un nuevo ID a la entidad
			void setID(std::size_t const id) noexcept { this->id_ = id; }
			// Obtenemos el ID de la entidad
			std::size_t GetID()
			{
				return id_;
			}
			// Plantilla para saber si la entidad tiene un componente del tipo CMP
			template <typename CMP>
			bool hasComponent() const noexcept
			{
				// Devuelve true si la máscara de componentes tiene el bit correspondiente al componente CMP
				return cmp_mask_ & cmp_info::template mask<CMP>();
			}
			//Plantilla para saber si la entidad tiene una tag
			template <typename TAG>
			bool hasTag() const noexcept
			{
				return tag_mask_ & tag_info::template mask<TAG>();
			}

		private:
			// Plantilla para añadir un tag a la entidad
			template <typename TAG>
			void addTag()
			{
				// Se actualiza la máscara de tags para indicar que esta entidad ahora tiene un tag del tipo TAG
				tag_mask_ |= tag_info::template mask<TAG>();
			}
			//Plantilla para obtener la referencia a un componente de la Entidad
			/*template<typename CMP>
			CMP& getComponent(Entity& entity) const noexcept
			{
				auto& storage = std::get<ComponentStorage<CMP>>(EntityManager::componentPools_);
				return *storage[entity.id_];
			}*/
			// Plantilla para añadir un componente a la entidad
			template <typename CMP>
			void addComponent()
			{
				// Actualiza la máscara de componentes para indicar que esta entidad ahora tiene un componente del tipo CMP
				cmp_mask_ |= cmp_info::template mask<CMP>();
			}
			// Resetea la entidad
			void reset() noexcept
			{
				cmp_mask_ = 0;
				tag_mask_ = 0;
				alive = false;
			}

			std::size_t id_{ 0 };
			mask_type_cmp cmp_mask_{}; // Máscara de componentes
			mask_type_tag tag_mask_{}; // Máscara de Tags
			bool alive{ true };
		};

		//Métodos manager de Entidades
		// 
		// Crear una nueva entidad y devolver la referencia
		Entity& createEntity() noexcept{
			assert(aliveCount < MAX_ENTITIES && "Límite de entidades vivas alcanzado");
			size_t newid;
			bool reused{ false };
			if (!freeEntityIDs.empty()) {
				//Reutilizo pos libre
				newid = freeEntityIDs.back();
				printf("Reutilizo entidad en pos %d\n", newid);
				freeEntityIDs.pop_back();
				reused = true;
			}
			else
			{
				newid = aliveCount;
			}
			Entity& newEntity = entities_[newid];

			if (!reused)
			{
				newEntity.setID(newid);  // Actualiza el ID al índice asignado
			}
			++aliveCount;
			newEntity.alive = true;
			return newEntity;
		}
		// Obtener un componente de una entidad
		// Plantilla para añadir un tag a una entidad del EntityManager
		template <typename TAG>
		void addTag(Entity& e)
		{
			if (e.template hasTag<TAG>()) return;

			e.template addTag<TAG>();
		}
		template <typename CMP>
		CMP& getComponent(Entity& entity) {
			//static_assert(cmp_info::template contains<CMP>(), "Componente no válido");
			assert(entity.hasComponent<CMP>() && "La entidad no tiene este componente");

			auto& storage = std::get<ComponentStorage<CMP>>(componentPools_);
			/*if (storage[entity.id_])
			{
				printf("El id es %d", entity.id_);
			}*/
			return *storage[entity.id_];
		}

		// Plantilla para anadir un componente a una entidad
		template <typename CMP, typename... Args>
		CMP& addComponent(Entity& entity, Args&&... args) {
			//Comprobamos si existe
			//static_assert(cmp_info::template contains<CMP>(), "Componente no válido");
			//Obtenemos el id
			/*const auto componentID = cmp_info::template id<CMP>();*/
			
			//Actualizamos la máscara
			entity.template addComponent<CMP>();

			// Asegurar almacenamiento del componente
			auto& storage = std::get<ComponentStorage<CMP>>(componentPools_);

			//if (storage.size() <= entity.id_) storage.resize(entity.id_ + 1);->antiguo

			//Devuelvo referencia al componente ya creado
			storage[entity.id_] = CMP(std::forward<Args>(args)...);
			/*if (storage[entity.id_] == std::nullopt)
			{
				printf("xd\n");
			}*/
			return *storage[entity.id_];
		}

		// Verificar si una entidad tiene un componente
		template <typename CMP>
		bool hasComponent(Entity& entity) const {
			return entity.template hasComponent<CMP>();
		}

		// Función que nos devuelve una tupla de componentes singleton
		template<typename SCMP, typename T = SCMP>
		[[nodiscard]] constexpr auto& getSingleton() noexcept
		{
			using ReturnType = std::conditional_t<std::is_const_v<T>, const SCMP, SCMP>;
			return std::get<ReturnType>(singletons_);
		}

		// Función que nos devuelve las entidades vivas
		template<typename T = Entity>
		auto getEntities() -> std::span<std::conditional_t<std::is_const_v<T>, const Entity, Entity>>
		{
			return std::span{ entities_.begin(), entities_.end()};
		}
		/*auto getEntities()
		{
			return liveEntities;
		}*/
		//Plantilla que nos devuelve una entidad dado una tag
		template<typename TAG>
		Entity* getEntitywithTag()
		{
			for (Entity& e : getEntities())
			{
				if (e.template hasTag<TAG>())
				{
					return &e;
				}
			}
			return nullptr;
		}

		// Plantilla para recorrer todas las entidades que tengan los componentes especificados
		template <typename CMPs,typename TAGs>
		void forEach(auto&& func) { forEachImpl(func, CMPs{},TAGs{}); }
		// Iterar sobre entidades con un componente específico
		template <typename... CMPs,typename... TAGs>
		void forEachImpl(auto&& func, MP::TypeList<CMPs...>, MP::TypeList<TAGs...>) {
			//static_assert(cmp_info::template contains<CMP>(), "Componente no válido");
			for (Entity& e : getEntities())
			{
				auto hasCMPs = (true && ... && e.template hasComponent<CMPs>());
				auto hasTAGs = (true && ... && e.template hasTag<TAGs>());
				if (hasCMPs && hasTAGs && e.alive)
				{
					func(e, getComponent<CMPs>(e)...);
				}
			}
		}
		// Plantilla para destruir un componente de una entidad
		template<typename CMP>
		void destroyComponent(Entity& e)
		{
			if (e.template hasComponent<CMP>())
			{
				auto& storage = std::get<ComponentStorage<CMP>>(componentPools_);
				if (e.id_ < storage.size()) {
					storage[e.id_] = std::nullopt; // Eliminar el componente
				}
				// Elimina el componente de la máscara de componentes
				e.cmp_mask_ &= ~cmp_info::template mask<CMP>();
			}
		}
		// Destruye todos los componentes de una entidad
		void destroyCMPs(Entity& e)
		{
			tuple_replace<CMPList> cmps{};
			MP::for_each_in_tuple(cmps, [&](auto cmpType)
				{
					using CMPType = decltype(cmpType);
					destroyComponent<CMPType>(e);
				});
		}
		// Eliminar una entidad
		void destroyEntity(size_t index) {
			//assert(index < aliveCount);
			assert(aliveCount > 0);
			//get entity
			Entity& e = entities_[index];

			// Eliminamos los componentes de la entidad
			destroyCMPs(e);

			// Reseteamos la entidad
			e.reset();

			//Actualizar la lista de entidades vivas
			//if (index != aliveCount - 1) { //si no esta en la última posición de las vivas
				// Mover la última entidad viva al lugar de la eliminada
				//std::swap(entities_[index], entities_[aliveCount - 1]);
				//Mantener ID
				//entities_[index].setID(index);
			//entities_[index].setID(aliveCount - 1);
			//}

			//Marcamos esta pos como libre
			freeEntityIDs.push_back(index);

			// Reducir el contador de entidades vivas
			aliveCount -= 1;
		}

		//Contador de entidades vivas
		std::size_t aliveCount{ 0 };
		//ids de entidades muertas, para reutilizarlas
		std::vector<size_t> freeEntityIDs{};
		//Array de Entidades
		std::array<Entity, MAX_ENTITIES> entities_{};

		// Almacenamiento para componentes -> tupla de arrays opcionales 
		// ( std::tuple<std::array<RenderComponent>, std::array<PhysicasComponent>...>
		template <typename CMP>
		///using ComponentStorage = std::vector<std::optional<CMP>>;
		using ComponentStorage = std::array<std::optional<CMP>,MAX_ENTITIES>;

		template <typename CMPList>
		struct ComponentPools;
		//extrae los tipos CMPs... del TypeList.
		//crea un alias Type que es una std::tuple donde cada tipo es ComponentStorage<CMP>.
		template <typename... CMPs>
		struct ComponentPools<MP::TypeList<CMPs...>> {
			using Type = std::tuple<ComponentStorage<CMPs>...>;
		};

		typename ComponentPools<CMPList>::Type componentPools_;

		//tupla de singletons
		singlestorage_t singletons_;
	};
} //namespace ETMG
