#include "ParticleSystem.hpp"
#include "World.hpp"

NParticleSystem::NParticleSystem()
{
    mTexture = nullptr;
    mNeedsVertexUpdate = true;
    mNeedsQuadUpdate = true;
}

void NParticleSystem::setTexture(std::string const& id)
{
    NParticleSystem::setTexture(NWorld::getResources().getTexture(id));
}

void NParticleSystem::setTexture(sf::Texture& texture)
{
    mTexture = &texture;
}

std::size_t NParticleSystem::addTextureRect(sf::IntRect const& rect)
{
    mTextureRects.push_back(rect);
}

void NParticleSystem::addAffector(Affector affector)
{
    mAffectors.push_back(affector);
}

void NParticleSystem::clearAffectors()
{
    mAffectors.clear();
}

void NParticleSystem::tick(sf::Time dt)
{
    mNeedsVertexUpdate = true;

    for (std::size_t i = 0; i < mParticles.size();)
    {
        updateParticle(mParticles[i], dt); // lifetime, move, rotate

        if (mParticles[i].passedLifetime < mParticles[i].totalLifetime)
        {
            for (std::size_t j = 0; j < mAffectors.size(); ++j)
            {
                if (mAffectors[i])
                {
                    mAffectors[i](mParticles[i],dt);
                }
            }
            ++i;
        }
        else
        {
            mParticles.erase(mParticles.begin() + i);
        }
    }
}

void NParticleSystem::render(sf::RenderTarget& target)
{
    if (mNeedsQuadUpdate)
    {
        computeQuads();
        mNeedsQuadUpdate = false;
    }

    if (mNeedsVertexUpdate)
    {
        computeVertices();
        mNeedsVertexUpdate = false;
    }

    sf::RenderStates states;
    states.texture = mTexture;
    target.draw(mVertices,states);
}

std::size_t NParticleSystem::getParticleCount() const
{
    return mParticles.size();
}

void NParticleSystem::clearParticles()
{
    mParticles.clear();
}

void NParticleSystem::emitParticle(Particle const& particle)
{
    mParticles.push_back(particle);
}

void NParticleSystem::updateParticle(Particle& particle, sf::Time dt)
{
    particle.passedLifetime += dt;

    particle.position += dt.asSeconds() * particle.velocity;
    particle.rotation += dt.asSeconds() * particle.rotationSpeed;
}

void NParticleSystem::computeVertices()
{
    mVertices.clear();
    for (std::size_t i = 0; i < mParticles.size(); ++i)
    {
        sf::Transform t;
        t.translate(mParticles[i].position);
        t.rotate(mParticles[i].rotation);
        t.scale(mParticles[i].scale);

        assert(mParticles[i].textureIndex == 0 || mParticles[i].textureIndex < mTextureRects.size());

        std::size_t tex = mParticles[i].textureIndex;
        for (std::size_t j = 0; j < 4; ++j)
        {
            sf::Vertex v;
            v.position = t.transformPoint(mQuads[tex][j].position);
            v.texCoords = mQuads[tex][j].texCoords;
            v.color = mParticles[i].color;

            mVertices.append(v);
        }
    }
}

void NParticleSystem::computeQuads()
{
	assert(mTexture);

	if (mTextureRects.empty())
	{
		mQuads.resize(1);
		computeQuad(mQuads[0], sf::IntRect(0, 0, mTexture->getSize().x, mTexture->getSize().y));
	}
	else
	{
		mQuads.resize(mTextureRects.size());
		for (std::size_t i = 0; i < mTextureRects.size(); ++i)
        {
            computeQuad(mQuads[i], mTextureRects[i]);
        }
	}
}

void NParticleSystem::computeQuad(Quad& quad, sf::IntRect const& tRect)
{
    sf::FloatRect rect(tRect);

	quad[0].texCoords = sf::Vector2f(rect.left,              rect.top);
	quad[1].texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
	quad[2].texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
	quad[3].texCoords = sf::Vector2f(rect.left,              rect.top + rect.height);

	quad[0].position = sf::Vector2f(-rect.width, -rect.height) / 2.f;
	quad[1].position = sf::Vector2f( rect.width, -rect.height) / 2.f;
	quad[2].position = sf::Vector2f( rect.width,  rect.height) / 2.f;
	quad[3].position = sf::Vector2f(-rect.width,  rect.height) / 2.f;
}


