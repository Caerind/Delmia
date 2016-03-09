
class MapActor : public NActor
{
    public:
        MapActor()
        {
            mMapComponent.create("tileset",{20,20},{32,32},NLayerComponent::Orthogonal);
            mIsoMapComponent.create("iso-tileset",{20,20},{180,150},NLayerComponent::Isometric);
            mHexMapComponent.create("hex-tileset",{20,20},{57,60},NLayerComponent::Hexagonal,28);

            sf::Vector2i coords;
            for (coords.x = 0; coords.x < 20; coords.x++)
            {
                for (coords.y = 0; coords.y < 20; coords.y++)
                {
                    mMapComponent.setTileId(coords,NMath::random(2,5));
                    mIsoMapComponent.setTileId(coords,NMath::random(2,5));
                    mHexMapComponent.setTileId(coords,NMath::random(1,5));
                }
            }

            mIsoMapComponent.setPosition(400,0);
            mHexMapComponent.setPosition(200,300);
        }

    private:
        NLayerComponent mMapComponent;
        NLayerComponent mIsoMapComponent;
        NLayerComponent mHexMapComponent;
};

class EActor : public NActor
{
    public:
        EActor()
        {
            attachComponent(&mSpriteComponent);

            mSpriteComponent.setTexture("icon");
            mSpriteComponent.setOrigin(64,64);
        }

        NSpriteComponent mSpriteComponent;
};

class Player : public NActor
{
    public:
        Player()
        {
            attachComponent(&mSpriteComponent);
            attachComponent(&mCameraComponent);

            mSpriteComponent.setTexture("icon");
            mSpriteComponent.setOrigin(64,64);
        }

        NSpriteComponent mSpriteComponent;
        NCameraComponent mCameraComponent;
};

class EState : public ah::State
{
    public:
        EState(ah::StateManager& manager) : ah::State(manager)
        {
            mRight.setTexture(NWorld::getResources().getTexture("icon"));
            mRight.setPosition(NWorld::getWindow().getSize().x-128,0);
            mRight.setCallback(SGUI::Button::Pressed,[&]()
            {
                ah::Application::close();
            });

            mLeft.setTexture(NWorld::getResources().getTexture("icon"));;
            mLeft.setPosition(0,NWorld::getWindow().getSize().y-128);
            mLeft.setCallback(SGUI::Button::Pressed,[&]()
            {
                auto e = NWorld::createActor<EActor>();
                std::string id;
                if (e != nullptr)
                {
                    e->setPosition(NVector(NMath::random(0.f,(float)NWorld::getWindow().getSize().x),NMath::random(0.f,(float)NWorld::getWindow().getSize().y)));
                    id = e->getId();
                }
                NWorld::setTimer(sf::seconds(2.f),[id](){NWorld::removeActor(id);});
            });

            mJoystick.setButtonTexture(NWorld::getResources().getTexture("joyButton"));
            mJoystick.setBackgroundTexture(NWorld::getResources().getTexture("joyBackground"));
            mJoystick.setPosition(sf::Vector2f(NWorld::getWindow().getSize().x - 150, NWorld::getWindow().getSize().y - 150));
            mJoystick.setDeltaMax(50);

            NWorld::createActor<MapActor>();

            mPlayer = NWorld::createActor<Player>();
            mPlayer->setPosition(100,100,100);
        }

        bool handleEvent(sf::Event const& event)
        {
            NWorld::addEvent(event);
            mLeft.handleEvent(event);
            mRight.handleEvent(event);
            mJoystick.handleEvent(event);
            return true;
        }

        bool update(sf::Time dt)
        {
            NWorld::tick(dt);

            mJoystick.update(dt);
            sf::Vector2f m = mJoystick.getDelta();
            if (m != sf::Vector2f() && mPlayer != nullptr)
            {
                mPlayer->setPosition(mPlayer->getPosition() + NVector::SFML2FToN(m * 300.f * dt.asSeconds()));
            }

            NWorld::update();

            ah::Application::getWindow().setDebugInfo("Actors",std::to_string(NWorld::getActorCount()));
            ah::Application::getWindow().setDebugInfo("Tickables",std::to_string(NWorld::getTickableCount()));
            ah::Application::getWindow().setDebugInfo("Renderables",std::to_string(NWorld::getRenderableCount()));
            ah::Application::getWindow().setDebugInfo("PPos",std::to_string(NWorld::getPointerPositionView(0).x) + " " + std::to_string(NWorld::getPointerPositionView(0).y));
            ah::Application::getWindow().setDebugInfo("SPos",std::to_string(NWorld::getPointerPositionScreen(0).x) + " " + std::to_string(NWorld::getPointerPositionScreen(0).y));

            return true;
        }

        void render(sf::RenderTarget& target, sf::RenderStates states)
        {
            NWorld::render(target);
            mLeft.render(target,states);
            mRight.render(target,states);
            mJoystick.render(target,states);
        }

    private:
        NActionTarget mController;
        SGUI::Button mLeft;
        SGUI::Button mRight;
        SGUI::Joystick mJoystick;
        Player::Ptr mPlayer;
};
