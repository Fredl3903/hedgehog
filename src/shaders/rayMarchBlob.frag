#version 330 core
    
    uniform vec3 uCamPos;
    uniform vec2 uRes;

    in vec4 color;
    out vec4 fragCol;

    float smin( float a, float b, float k ) {
        k *= 4.0;
        float h = max( k-abs(a-b), 0.0 )/k;
        return min(a,b) - h*h*k*(1.0/4.0);
    }


    float sdfSphere(vec3 p, float r) {
        return length(p) - r;
    }
    float sdfBox(vec3 p, vec3 b) {
        vec3 q = abs(p) - b;
        return length(max(q, 0.0)) + min(max(q.x, max(q.y,q.z)),0.0);
    }

    void main() {

        const float focalLength = 2.0f;

        const vec3 s1Centre = vec3(-0.8f, 0.6f, 0.5f);
        const float s1R = 0.3;

        const vec3 s2Centre = vec3(-0.3f, -0.5f, 0.7f);
        const float s2R = 0.14;

        const vec3 s3Centre = vec3(0.9f, -0.5, 0.0f);
        const float s3R = 0.4;

        const vec3 s4Centre = vec3(0.5f, 0.7f, 0.4f);
        const float s4R = 0.2;

        const vec3 boxCentre = vec3(0.0f, 0.0f, 0.0f);
        const vec3 boxB = vec3(0.3f, 0.3f, 0.3f);
        
        vec2 uv = (gl_FragCoord.xy / uRes) - 0.5;
        float aspect = uRes.x / uRes.y;
        vec3 rHat = normalize(vec3(uv.x * aspect, uv.y, -focalLength));

        float sd;
        float t = 0; // distance along the ray
        for (int i = 0; i < 100; i++) {
            vec3 p = uCamPos + t * rHat;
            sd = sdfSphere(p - s1Centre, s1R);
            sd = smin(sd, sdfSphere(p - s2Centre, s2R), 0.18);
            sd = smin(sd, sdfSphere(p - s3Centre, s3R), 0.21);
            sd = smin(sd, sdfSphere(p - s4Centre, s4R), 0.18);
            sd = smin(sd, sdfBox(p - boxCentre, boxB), 0.18);
            sd = smin(sd, p.y + 0.8, 0.1);

            if (sd < 0.0001) {
                // hit the surface :)
                break;
            }

            t += sd;
        }

        fragCol = vec4(0.9, 0.7, 1.0, 1.0) * (8.0 / (-2.0 + 1.0 * t * t)) * (8.0 / (-2.0 + 1.0 * t * t));

    }