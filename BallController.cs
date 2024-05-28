using UnityEngine;
using TMPro;
using System.Collections;

public class BallController : MonoBehaviour
{
    public GameManager gameManager; // Reference to the GameManager script

    [Header("References")]
    public Rigidbody rb;
    public TextMeshProUGUI strokeCountText;
    public Canvas gameOverCanvas;
    public TextMeshProUGUI finalScoreText;
    public Transform holeTransform;

    Camera mainCamera;

    [Header("Ball settings")]
    public float StopVelocity;
    public float ShotPower;
    public float MaxPower;
    public float RespawnThreshold = -10f;

    bool isAiming;
    bool isIdle;
    bool isShooting;

    Vector3? worldpoint;
    Vector3 startingPosition;
    int strokeCount = 0;
    bool canMove = true; // Flag to determine if the ball can move

// On awake set the camera, set the maxAngular Velocity to 1000, aiming to false, and set its starting position
    void Awake()
    {
        mainCamera = Camera.main;
        rb.maxAngularVelocity = 1000;
        isAiming = false;
        startingPosition = transform.position;
    }
// Reseting the balls position and its velocity 
    public void ResetBall()
    {
        rb.velocity = Vector3.zero;
        rb.angularVelocity = Vector3.zero;
        transform.position = startingPosition;

        StartCoroutine(DelayedEnableCoroutine());
    }

    IEnumerator DelayedEnableCoroutine()
    {
        yield return new WaitForFixedUpdate();
        isIdle = true;
    }

    IEnumerator DelayedEnable()
    {
        yield return new WaitForFixedUpdate();
        isIdle = true;
    }
// if your able to move and are not already moving allowing aiming to be true and when it is set isShooting to be true
    void Update()
    {
        if (canMove && rb.velocity.magnitude < StopVelocity)
        {
            ProcessAim();
            // setting isaming to true when you are holding the mouse button down
            if (Input.GetMouseButtonDown(0))
            {
                if (isIdle) isAiming = true;
            }
            // setting is shooting to true when you release the mouse button
            if (Input.GetMouseButtonUp(0))
            {
                isShooting = true;
            }
        }
// calling the respawn function when the ball falls over the edge 
        if (transform.position.y < RespawnThreshold)
        {
            Respawn();
        }
    }

    void FixedUpdate()
    {
        // checking if the ball is already moving 
        if (canMove && rb.velocity.magnitude < StopVelocity)
        {
            stop();
        }
        // checking if you are shooting the ball 
        if (isShooting)
        {
            Shoot(worldpoint.Value);
            isShooting = false;
        }
    }
// setting the animing and the calling the "CastMouseClickRay" function 
    private void ProcessAim()
    {
        if (!isAiming && !isIdle) return;

        worldpoint = CastMouseClickRay();

        if (!worldpoint.HasValue) return;
    }
// cast a raycast where your mouse position is 
    private Vector3? CastMouseClickRay()
    {
        Ray ray = mainCamera.ScreenPointToRay(Input.mousePosition);

        if (Physics.Raycast(ray, out RaycastHit hit)) return hit.point;
        else return null;
    }
// Stopping the ball when it hits a certain velocity 
    private void stop()
    {
        rb.velocity = Vector3.zero;
        rb.angularVelocity = Vector3.zero;

        isIdle = true;
    }
// shooting the ball and stopping it from shooting again right after 
    private void Shoot(Vector3 point)
    {
        isAiming = false;

        Vector3 horizontalWorldPoint = new Vector3(point.x, transform.position.y, point.z);

        Vector3 direction = (horizontalWorldPoint - transform.position).normalized;
        
        float strength = Vector3.Distance(transform.position, horizontalWorldPoint);
        float SpeedCap =  strength * ShotPower;
        // Setting a speed cap to the ball 
        if(SpeedCap < -7000)
        {
            SpeedCap = -7000;
        }
        Debug.Log(SpeedCap);
        rb.AddForce(SpeedCap * direction);
        
        strokeCount++;
        // Update the stroke count in the GameManager
        gameManager.IncrementStrokeCount();
    }
// when this is called respaswn the ball back at the start of the hole
    private void Respawn()
    {
        transform.position = startingPosition;
        rb.velocity = Vector3.zero;
        rb.angularVelocity = Vector3.zero;

        isIdle = true;
        

        // Update the stroke count in the GameManager
        gameManager.IncrementStrokeCount();
    }
    // detect if the ball collides with the hole and end the game when it does  
    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Obstacle"))
        {
            rb.velocity = Vector3.zero;
            rb.angularVelocity = Vector3.zero;
            isIdle = true;
            enabled = false;

            // Notify the GameManager that the game has ended
            gameManager.EndGame();
        }
    }

    // Method to enable/disable ball movement based on button click
    public void ToggleBallMovement()
    {
        canMove = !canMove;
    }
}
