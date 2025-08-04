  vertPos = vec3(modelMat * vec4(aPos, 1.0f));
  vertNorm = mat3(transpose(inverse(modelMat))) * aNorm;
  vertColor = aColor;
  vertTex = aTex;
  
  gl_Position = projMat * viewMat * vec4(vertPos, 1.0);