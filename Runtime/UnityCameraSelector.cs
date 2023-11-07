using UnityEngine;
using System;
using Reinterop;

namespace CesiumForUnity
{
    [Reinterop]
    internal class UnityCameraSelector : MonoBehaviour
    {
        public static UnityCameraSelector Instance;

            [SerializeField]
            private Camera _camera;


        private void Awake()
        {
            Instance = this;
        }
        

        public static Camera GetCamera()
        {
            if(Instance!=null)
                return Instance._camera;

            return null;
        }

        public void ExposeToCPP()
        {
            Console.WriteLine("yo");
        }

        private void OnDestroy()
        {
            Instance = null;
        }
    }

}