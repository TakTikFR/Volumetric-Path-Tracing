#include <iostream>
#include <optional>

#include "box.hh"
#include "camera.hh"
#include "dielectric_texture.hh"
#include "diffuse_light.hh"
#include "homogeneous_volume.hh"
#include "image.hh"
#include "interval.hh"
#include "isotropic.hh"
#include "metal_texture.hh"
#include "noise_texture.hh"
#include "object.hh"
#include "point3.hh"
#include "ray.hh"
#include "scene.hh"
#include "sphere.hh"
#include "uniforme_texture.hh"
#include "utils.hh"
#include "vector3.hh"

constexpr int MAX_DEPTH = 100;
constexpr int SAMPLES = 500;

RGB rayColor(const Ray &ray, const Scene &scene, int depth)
{
    if (depth <= 0)
        return RGB(0, 0, 0);

    interval ray_int(0.001, infinity);
    Hit closestHit;
    Object *hitObj = nullptr;

    for (Object *obj : scene.objects)
    {
        std::optional<Hit> result = obj->intersect(ray, ray_int);
        if (result)
        {
            ray_int.max = result->t;
            closestHit = *result;
            hitObj = obj;
        }
    }

    if (!hitObj)
    {
        return RGB(0, 0, 0);
        /*Vector3 unitDir = ray.direction.normalize();*/
        /*double t = 0.5 * (unitDir.y + 1.0);*/
        /*return RGB(static_cast<uint8_t>((1.0 - t) * 255.0 + t * 127.5),*/
        /*           static_cast<uint8_t>((1.0 - t) * 255.0 + t * 179.0),*/
        /*           static_cast<uint8_t>((1.0 - t) * 255.0 + t * 255.0));*/
    }

    Texture_Material *material = hitObj->getMaterial();
    RGB emitted = material->emitted(closestHit.point);
    RGB color;
    Ray reboundRay;
    if (material->scatter(ray, closestHit, color, reboundRay))
        return emitted + (color * rayColor(reboundRay, scene, depth - 1));
    else
        return emitted;
}

int main()
{
    Scene scene;

    // ─── MATÉRIAUX DE LA PIÈCE ──────────────────────────────────────────────
    auto *white = new UniformeTexture(
        RGB(180, 180, 180)); // Pas de blanc pur à 255 (trop brillant)
    auto *red = new UniformeTexture(RGB(165, 25, 25));
    auto *green = new UniformeTexture(RGB(25, 115, 25));

    // ─── LES MURS DE LA PIÈCE (Des Box plates) ──────────────────────────────
    // Sol
    scene.addObject(
        new Box(Point3(-5.0, 0.0, -5.0), Point3(5.0, 0.1, 5.0), white));
    // Plafond
    scene.addObject(
        new Box(Point3(-5.0, 4.9, -5.0), Point3(5.0, 5.0, 5.0), white));
    // Mur du fond
    scene.addObject(
        new Box(Point3(-5.0, 0.0, -5.1), Point3(5.0, 5.0, -5.0), white));
    // Mur Gauche (Rouge)
    scene.addObject(
        new Box(Point3(-5.1, 0.0, -5.0), Point3(-5.0, 5.0, 5.0), red));
    // Mur Droit (Vert)
    scene.addObject(
        new Box(Point3(5.0, 0.0, -5.0), Point3(5.1, 5.0, 5.0), green));

    // ─── LA LUMIÈRE (Sphère au plafond) ─────────────────────────────────────
    // Intensité de 10.0 pour bien éclairer toute la pièce
    auto *light_mat = new DiffuseLight(RGB(255, 255, 255), 15.0);
    scene.addObject(
        new Box(Point3(-2.0, 4.89, -4.5), Point3(2.0, 5.0, -0.5), light_mat));

    // ─── LE BLOC CENTRAL (Pour tester ton Perlin) ───────────────────────────
    // Pour l'instant, c'est un bloc gris clair. C'est ici que tu mettras ta
    // NoiseTexture !
    // auto *block_mat = new Isotropic(RGB(255, 255, 255), 4.0);
    // scene.addObject(
    //    new Box(Point3(-1.0, 0.1, -3.0), Point3(1.0, 2.1, -1.0), block_mat));

    auto *block_mat = new Isotropic(RGB(255, 255, 255), 1.0);
    auto *box = new Box(Point3(-1.0, 0.1, -3.0), Point3(1.0, 2.1, -1.0), white);
    auto *obj = new HomogeneousVolume(box, 1.0, block_mat);
    scene.addObject(obj);

    // ─── CAMÉRA ET IMAGE ────────────────────────────────────────────────────
    constexpr int width = 500; // Résolution basse pour tester vite
    constexpr int height = 500; // Format carré classique pour une Cornell Box
    Image image(width, height);

    // On recule la caméra et on regarde le centre de la pièce
    Camera camera(Point3(0.0, 2.5, 3.5), // Position de la caméra
                  Point3(0.0, 2.0, -2.5), // Cible (le centre du bloc)
                  Vector3(0.0, 1.0, 0.0), // Up
                  60.0, 1.0, 1.0 // Alpha, Beta (1.0 pour format carré), z_min
    );

    constexpr double focalLength = 1.0;
    constexpr double viewportHeight = 2.0;
    constexpr double viewportWidth = viewportHeight; // Carré

    const Vector3 viewportU = Vector3(viewportWidth, 0.0, 0.0);
    const Vector3 viewportV = Vector3(0.0, -viewportHeight, 0.0);
    const Vector3 pixelDeltaU = viewportU / static_cast<double>(width);
    const Vector3 pixelDeltaV = viewportV / static_cast<double>(height);

    const Vector3 viewportUpperLeft = Vector3(camera.center)
        - Vector3(0.0, 0.0, focalLength) - viewportU / 2.0 - viewportV / 2.0;

    const Vector3 pixel00 =
        viewportUpperLeft + (pixelDeltaU + pixelDeltaV) * 0.5;

    // ─── BOUCLE DE RENDU ────────────────────────────────────────────────────
    std::cout << "Début du rendu : " << width << "x" << height << " pixels."
              << std::endl;

#pragma omp parallel for schedule(dynamic)
    for (int j = 0; j < height; ++j)
    {
        std::cout << "\rLignes restantes : " << (height - j) << ' '
                  << std::flush;
        for (int i = 0; i < width; ++i)
        {
            double r = 0, g = 0, b = 0;
            for (int s = 0; s < SAMPLES; ++s)
            {
                double u = static_cast<double>(i) + random_double();
                double v = static_cast<double>(j) + random_double();
                const Vector3 pixelPos =
                    pixel00 + pixelDeltaU * u + pixelDeltaV * v;
                const Ray ray(camera.center, pixelPos - Vector3(camera.center));

                RGB c = rayColor(ray, scene, MAX_DEPTH);
                r += c.r;
                g += c.g;
                b += c.b;
            }
            // On fait juste la moyenne, sans se soucier du uint8_t ici
            image.set(i, j, RGB(r / SAMPLES, g / SAMPLES, b / SAMPLES));
        }
    }

    std::cout << "\nRendu terminé !\n";
    image.save("output.ppm");
    std::cout << "Image sauvegardée dans output.ppm\n";

    return 0;
}
